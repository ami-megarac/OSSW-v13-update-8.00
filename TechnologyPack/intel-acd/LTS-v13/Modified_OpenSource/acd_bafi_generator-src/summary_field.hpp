
#pragma once
#include "aer.hpp"
#include "mca_defs.hpp"
#include "tor_defs.hpp"
#include "tor_defs_icx.hpp"
#include "utils.hpp"
#include "tsc.hpp"

#include <array>
#include <map>
#include <set>
#include <string>
#include <variant>

enum class MapType
{
    SINGLE,
    DOUBLE
};

/*
SummaryField abstraction structure:
                |--------------------------------------------------------------------|
                |                        SummaryField                                |
                |--------------------------------------------------------------------|
                |(implements base operations on maps like insert(), at(), contains())|
                |--------------------------------------------------------------------|
                                                |
                            |-------------------------------------------|
                            V                                           V
        |----------------------------------|       |--------------------------------------------------------------|             
        | McaSummaryField, TorSummaryField |       |                   GenericSummaryField                        |  
        |----------------------------------|       |--------------------------------------------------------------|
        |(implements fillEntries() function|       |(implements fillEntries() and fillEntries1D() which is used by|
        |     for both 1D and 2D map)      |       |   all child classes, enables overloading fillEntries2D())    |
        |----------------------------------|       |--------------------------------------------------------------|
                        |                                                         |
                        |                            |----------------------------|----------------------------------|
                        |                            V                            V                                  V
                        |                |----------------------------|   |----------------------------|    |-----------------------------|
                        |                |      AerSummaryField       |   |      TscSummaryField       |    |      ThermSummaryField      |
                        |                |----------------------------|   |----------------------------|    |-----------------------------|
                        |                | implements fillEntries2D(),|   |(implements fillEntries2D(),|    |(implements fillEntries2D(), |
                        |                | specific for AER registers |   |specific for TSC registers) |    |specific for Therm registers)|
                        |                |----------------------------|   |----------------------------|    |-----------------------------|
                        |                            |                            |                                  |
                        |                            |                            |                                  |                       
                        |                            |                            |                                  |                        
                        |------------------------------------------|-------------------------------------------------|
                                                                   |
                                                                   V
                        |------------------------------------------------------------------------------------------------------------|
                        |  TorDieSummaryField, TorBaseSummaryField, McaDieSummaryField, McaBaseSummaryField, AerDieSummaryField,     |
                        | AerBaseSummaryField, TscDieSummaryField, TscBaseSummaryField, ThermDieSummaryField, ThermBaseSummaryField, |
                        |------------------------------------------------------------------------------------------------------------|                               
                        |            Child classes overloading constructor, seting internal map type to either 1D or 2D              |         
                        |------------------------------------------------------------------------------------------------------------|
*/      
template <typename T> class SummaryField
{
  public:
    using map1D = std::map<uint32_t, T>;
    using map2D = std::map<uint32_t, std::map<std::string, T>>;

    std::variant<map1D, map2D> data;
    SummaryField(MapType type)
    {
        switch (type)
        {
            case MapType::SINGLE:
                data = map1D();
                break;
            case MapType::DOUBLE:
                data = map2D();
                break;
            default:
                throw std::logic_error("incorrect type provided");
                break;
        }
    };

    bool holds2D()
    {
        return std::holds_alternative<map2D>(data);
    }

    std::vector<T> flatten()
    {
        std::vector<T> flattened;
        if (holds2D())
        {
            for (const auto& [first, second] : std::get<map2D>(data))
            {
                for (const auto& [firstElem, secondElem] : second)
                {
                    flattened.push_back(secondElem);
                }
            }
        }
        else
        {
            for (const auto& [first, second] : std::get<map1D>(data))
            {
                flattened.push_back(second);
            }
        }

        return flattened;
    }

    void insert(uint32_t cpuIdx, T in)
    {
        if (holds2D())
        {
            throw std::logic_error("Incorrect number of arguments provided");
        }

        std::get<map1D>(data)[cpuIdx] = in;
    }

    void insert(uint32_t cpuIdx, std::string dieIdx, T in)
    {
        if (!holds2D())
        {
            throw std::logic_error("Incorrect number of arguments provided");
        }

        std::get<map2D>(data)[cpuIdx][dieIdx] = in;
    }

    bool contains(uint32_t cpuIdx)
    {
        if (holds2D())
        {
            return std::get<map2D>(data).count(cpuIdx) > 0;
        }
        else
        {
            return std::get<map1D>(data).count(cpuIdx) > 0;
        }
    }

    bool contains(uint32_t cpuIdx, std::string dieIdx)
    {
        if (holds2D())
        {
            if (std::get<map2D>(data).count(cpuIdx) == 0)
            {
                return false;
            }
            return std::get<map2D>(data)[cpuIdx].count(dieIdx) > 0;
        }
        else
        {
            throw std::logic_error("Incorrect number of arguments provided");
        }
    }

    T& at(uint32_t cpuIdx)
    {
        if (holds2D())
        {
            throw std::logic_error("Incorrect number of arguments provided");
        }

        return std::get<map1D>(data).at(cpuIdx);
    }

    T& at(uint32_t cpuIdx, std::string dieIdx)
    {
        if (!holds2D())
        {
            throw std::logic_error("Incorrect number of arguments provided");
        }

        return std::get<map2D>(data).at(cpuIdx).at(dieIdx);
    }
};

class TorSummaryField : public SummaryField<std::vector<std::shared_ptr<TORDataGen>>>
{

  protected:
    using map1D = std::map<uint32_t, std::vector<std::shared_ptr<TORDataGen>>>;
    using map2D = std::map<uint32_t, std::map<std::string, std::vector<std::shared_ptr<TORDataGen>>>>;

    std::map<uint32_t, SocketCtx> ctxData;

  public:
    TorSummaryField(MapType type) : SummaryField<std::vector<std::shared_ptr<TORDataGen>>>(type)
    {
    }

    void insertSocketCtx(uint32_t cpuIdx, SocketCtx in)
    {
        ctxData.insert(std::pair<uint32_t, SocketCtx>(cpuIdx, in));
    }

    SocketCtx& atSocketCtx(uint32_t cpuIdx)
    {
        return ctxData.at(cpuIdx);
    }

    void fillEntries(json& torReport, json& summaryReport,
                     std::function<void(std::shared_ptr<TORDataGen>, json&, json&, json&, SocketCtx, std::optional<std::string>)>
                         decodeTorEntry,
                     std::function<void(json&, json&, SocketCtx)> processSocketTor)
    {
        for (const auto& [socketId, ctx]: ctxData)
        {
            std::string socket = "socket" + std::to_string(socketId);
            json& summaryEntry = summaryReport[socket];
            json& socketEntry = torReport[socket];
            json errorsPerSocket;

            processSocketTor(summaryEntry, errorsPerSocket, ctx);
            
            if (this->holds2D())
            {
                socketEntry = json::object();
                if (std::get<map2D>(data).count(socketId) == 0)
                {
                    continue;
                }

                for (const auto& [die, dieData] : std::get<map2D>(data)[socketId])
                {
                    json& dieEntry = socketEntry[die];
                    dieEntry = json::array();

                    for (const auto& torEntry : dieData)
                    {
                        decodeTorEntry(torEntry, errorsPerSocket, dieEntry, summaryEntry, ctx, die);
                    }
                }
            }
            else
            {
                socketEntry = json::array();
                if (std::get<map1D>(data).count(socketId) == 0)
                {
                    continue;
                }

                for (const auto& torEntry : std::get<map1D>(data)[socketId])
                {
                    decodeTorEntry(torEntry, errorsPerSocket, socketEntry, summaryEntry, ctx, std::nullopt);
                }
            }
        }
    }
};
class TorDieSummaryField : public TorSummaryField
{
  public:
    TorDieSummaryField() : TorSummaryField(MapType::DOUBLE)
    {
    }
};

class TorBaseSummaryField : public TorSummaryField
{
  public:
    TorBaseSummaryField() : TorSummaryField(MapType::SINGLE)
    {
    }
};

class McaSummaryField : public SummaryField<std::vector<MCAData>>
{
  private:
    json createMceSummaryString(std::vector<std::array<std::string, 5>> mceSummaryData)
    {
        json mcaEntry;
        for (auto const& mcaData : mceSummaryData)
        {
            std::stringstream ss1;
            std::stringstream ss2;
            if (mcaData[0] != "")
            {
                ss1 << "bank " << mcaData[0];
            }

            ss1 << " |";

            if (mcaData[1] != "" && mcaData[2] != "")
            {
                ss1 << " " << mcaData[1] << ", " << mcaData[2];
            }
            if (mcaData[3] != "")
            {
                ss2 << mcaData[3];
            }
            if (mcaData[4] != "")
            {
                if (mcaData[3] != "")
                    ss2 << ", ";
                ss2 << mcaData[4];
            }
            mcaEntry[ss1.str()] = ss2.str();
        }
        return mcaEntry;
    }

    std::optional<json> generateMcaSummary(json& mcaData)
    {
        std::vector<std::array<std::string, 5>> mceSummary;
        for (auto const& entry : mcaData)
        {
            std::array<std::string, 5> summaryData = {"", "", "", "", ""};
            summaryData[0] = entry["Bank"];
            if (entry["Status_decoded"].contains("MCACOD"))
            {
                summaryData[1] = "MCACOD: " + std::string(entry["Status_decoded"]["MCACOD"]);
            }
            if (entry["Status_decoded"].contains("MSCOD"))
            {
                summaryData[2] = "MSCOD: " + std::string(entry["Status_decoded"]["MSCOD"]);
            }
            if (entry["Status_decoded"].contains("MCACOD_decoded"))
            {
                summaryData[3] = "MCACOD_decoded: " + std::string(entry["Status_decoded"]["MCACOD_decoded"]);
            }
            if (entry["Status_decoded"].contains("MSCOD_decoded"))
            {
                summaryData[4] = "MSCOD_decoded: " + std::string(entry["Status_decoded"]["MSCOD_decoded"]);
            }
            if (entry["Status_decoded"].contains("INSTANCE_ID_decoded"))
            {
                summaryData[4] = "INSTANCE_ID_decoded: " + std::string(entry["Status_decoded"]["INSTANCE_ID_decoded"]);
            }
            if (entry["Status_decoded"].contains("MSEC_FW_decoded"))
            {
                summaryData[4] = "MSEC_FW_decoded: " + std::string(entry["Status_decoded"]["MSEC_FW_decoded"]);
            }
            mceSummary.push_back(summaryData);
        }

        if(mceSummary.size() == 0)
        {
            return std::nullopt;
        }

        return createMceSummaryString(mceSummary);
    }

  public:
    McaSummaryField(MapType type) : SummaryField<std::vector<MCAData>>(type)
    {
    }

    void fillEntries(std::function<json(std::vector<MCAData>)> func, json& report, json& summaryReport)
    {

        if (this->holds2D())
        {
            for (const auto& [first, second] : std::get<map2D>(data))
            {
                std::string socket = "socket" + std::to_string(first);
                json mcaSummary;

                for (const auto& [firstElem, secondElem] : second)
                {
                    std::string die = firstElem;
                    json processedValue = func(secondElem);

                    report[socket][die] = processedValue;
                    
                    auto generatedSummary = generateMcaSummary(processedValue);
                    if(generatedSummary)
                    {
                        mcaSummary["MCE"][die] = *generatedSummary;
                    }
                }
                if (!mcaSummary.empty())
                {
                    summaryReport[socket].push_back(mcaSummary);
                }
            }
        }
        else
        {
            for (const auto& [first, second] : std::get<map1D>(data))
            {
                std::string socket = "socket" + std::to_string(first);
                json mcaSummary;
                json processedValue = func(second);
                auto generatedSummary = generateMcaSummary(processedValue);
                if(generatedSummary)
                {
                    mcaSummary["MCE"] = *generatedSummary;
                }

                report[socket] = processedValue;

                if (!mcaSummary.empty())
                {
                    summaryReport[socket].push_back(mcaSummary);
                }
            }
        }
    }
};

class McaDieSummaryField : public McaSummaryField
{
  public:
    McaDieSummaryField() : McaSummaryField(MapType::DOUBLE)
    {
    }
};

class McaBaseSummaryField : public McaSummaryField
{
  public:
    McaBaseSummaryField() : McaSummaryField(MapType::SINGLE)
    {
    }
};

template <typename T> class GenericSummaryField : public SummaryField<T>
{
    using map1D = std::map<uint32_t, T>;

  public:
    GenericSummaryField(MapType type) : SummaryField<T>(type)
    {
    }

    virtual void fillEntries1D(std::function<json(T)> func, json& report)
    {
        for (const auto& [first, second] : std::get<map1D>(this->data))
        {
            std::string socket = "socket" + std::to_string(first);

            json processedValue = func(second);
            if (!processedValue.empty())
            { 
                report[socket].push_back(processedValue);
            }
        }
    }

    virtual void fillEntries2D(std::function<json(T)> func, json& report) = 0;

    virtual void fillEntries(std::function<json(T)> func, json& report)
    {
        if (this->holds2D())
        {
            fillEntries2D(func, report);
        }
        else
        {
            fillEntries1D(func, report);
        }
    }
};

class AerSummaryField : public GenericSummaryField<std::vector<AerData>>
{
    using map2D = std::map<uint32_t, std::map<std::string, std::vector<AerData>>>;

  public:
    AerSummaryField(MapType type) : GenericSummaryField<std::vector<AerData>>(type)
    {
    }
    virtual void fillEntries2D(std::function<json(std::vector<AerData>)> func, json& report)
    {
        for (const auto& [first, second] : std::get<map2D>(this->data))
        {
            std::string socket = "socket" + std::to_string(first);
            std::vector<AerData> flattened;

            for (const auto& [firstElem, secondElem] : second)
            {
                for (const auto& elem : secondElem)
                {
                    flattened.push_back(elem);
                }
            }

            json processedValue = func(flattened);
            if (!processedValue.empty())
            {
                report[socket].push_back(processedValue);
            }
        }
    }
};

class AerDieSummaryField : public AerSummaryField
{
  public:
    AerDieSummaryField() : AerSummaryField(MapType::DOUBLE)
    {
    }
};

class AerBaseSummaryField : public AerSummaryField
{
  public:
    AerBaseSummaryField() : AerSummaryField(MapType::SINGLE)
    {
    }
};
class TscSummaryField : public GenericSummaryField<TscData>
{
  public:
    TscSummaryField(MapType type) : GenericSummaryField<TscData>(type)
    {
    }
    virtual void fillEntries2D(std::function<json(TscData)> func, json& report)
    {
        for (const auto& [first, second] : std::get<map2D>(data))
        {
            std::string socket = "socket" + std::to_string(first);
            if (second.count("io0") == 0)
            {
                throw std::logic_error("Socket section does not contain io0 die section");
            }

            json processedValue = func(second.at("io0"));
            if (processedValue.empty() && (second.count("soc") > 0))
            {
                processedValue = func(second.at("soc"));
            }

            if (!processedValue.empty())
            { 
                report[socket].push_back(processedValue);
            }
        }
    }
};

class TscDieSummaryField : public TscSummaryField
{
  public:
    TscDieSummaryField() : TscSummaryField(MapType::DOUBLE)
    {
    }
};

class TscBaseSummaryField : public TscSummaryField
{
  public:
    TscBaseSummaryField() : TscSummaryField(MapType::SINGLE)
    {
    }
};

class ThermSummaryField : public GenericSummaryField<PackageThermStatus>
{
  public:
    ThermSummaryField(MapType type) : GenericSummaryField<PackageThermStatus>(type)
    {
    }
    void fillEntries2D(std::function<json(PackageThermStatus)> func, json& report)
    {
        for (const auto& [first, second] : std::get<map2D>(data))
        {
            std::string socket = "socket" + std::to_string(first);
            json socketTherm;
            for (const auto& [firstElem, secondElem] : second)
            {
                socketTherm[firstElem] = func(secondElem)["Package_Therm_Status"];
            }

            json socketThermStatus;
            socketThermStatus["Package_Therm_Status"] = socketTherm;
            if (!socketTherm.empty())
            { 
                report[socket].push_back(socketThermStatus);
            }
        }
    }
};

class ThermDieSummaryField : public ThermSummaryField
{
  public:
    ThermDieSummaryField() : ThermSummaryField(MapType::DOUBLE)
    {
    }
};

class ThermBaseSummaryField : public ThermSummaryField
{
  public:
    ThermBaseSummaryField() : ThermSummaryField(MapType::SINGLE)
    {
    }
};

using MCA = McaSummaryField;
using Aer = AerSummaryField;
using TSC = TscSummaryField;
using Therm = ThermSummaryField;
using Tor = TorSummaryField;

using MCAPointer = std::shared_ptr<MCA>;
using AerPointer = std::shared_ptr<Aer>;
using TSCPointer = std::shared_ptr<TSC>;
using ThermPointer = std::shared_ptr<Therm>;
using TorPointer = std::shared_ptr<Tor>;

using MCAGeneric = McaBaseSummaryField;
using AerGeneric = AerBaseSummaryField;
using TSCGeneric = TscBaseSummaryField;
using ThermGeneric = ThermBaseSummaryField;
using TorGeneric = TorBaseSummaryField;

using MCAGenericPointer = std::shared_ptr<MCAGeneric>;
using AerGenericPointer = std::shared_ptr<AerGeneric>;
using TSCGenericPointer = std::shared_ptr<TSCGeneric>;
using ThermGenericPointer = std::shared_ptr<ThermGeneric>;
using TorGenericPointer = std::shared_ptr<TorGeneric>;

using MCABhs = McaDieSummaryField;
using AerBhs = AerDieSummaryField;
using TSCBhs = TscDieSummaryField;
using ThermBhs = ThermDieSummaryField;
using TorBhs = TorDieSummaryField;

using MCABhsPointer = std::shared_ptr<MCABhs>;
using AerBhsPointer = std::shared_ptr<AerBhs>;
using TSCBhsPointer = std::shared_ptr<TSCBhs>;
using ThermBhsPointer = std::shared_ptr<ThermBhs>;
using TorBhsPointer = std::shared_ptr<TorBhs>;
