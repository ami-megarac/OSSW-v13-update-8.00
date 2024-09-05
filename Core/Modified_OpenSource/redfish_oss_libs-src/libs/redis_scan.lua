local redis_scan = require("redis")
---------------------------------------------------------------------------------------------

local request = {}

function request.multibulk(client, command, ...)
	--print("request.multibulk 1")
    local args = {...}
	--print("2")
    local argsn = #args
	--print("3")
    local buffer = { true, true }

	--print("4")
    if argsn == 1 and type(args[1]) == 'table' then
        argsn, args = #args[1], args[1]
    end

	--print("5")
    buffer[1] = '*' .. tostring(argsn + 1) .. "\r\n"
    buffer[2] = '$' .. #command .. "\r\n" .. command .. "\r\n"

	--print("buffer[1] : " .. buffer[1])
	--print("buffer[2] : " .. buffer[2])
	
    local table_insert = table.insert
    for _, argument in pairs(args) do
        local s_argument = tostring(argument)
        table_insert(buffer, '$' .. #s_argument .. "\r\n" .. s_argument .. "\r\n")
    end

	--print("buffer : " .. buffer[1])
	--print("buffer : " .. buffer[2])
	--print("buffer : " .. buffer[3])
    client.network.write(client, table.concat(buffer))
end

local function scan_request(client, command, ...)
    local args, req, params = {...}, { }, nil

    if command == 'SCAN' then
        table.insert(req, args[1])
        params = args[2]
    else
        table.insert(req, args[1])
        table.insert(req, args[2])
        params = args[3]
    end

	if params and params.match then
        table.insert(req, 'MATCH')
        table.insert(req, args[3])
		table.insert(req, 'COUNT')
        table.insert(req, args[5])
    end
	
	--[[
    if params and params.match then
        table.insert(req, 'MATCH')
        table.insert(req, params.match)
    end

    if params and params.count then
        table.insert(req, 'COUNT')
        table.insert(req, params.count)
    end
	]]--
    request.multibulk(client, command, req)
end

local zscan_response = function(reply, command, ...)
    local original, new = reply[2], { }
    for i = 1, #original, 2 do
        table.insert(new, { original[i], tonumber(original[i + 1]) })
    end
    reply[2] = new

    return reply
end

local hscan_response = function(reply, command, ...)
    local original, new = reply[2], { }
    for i = 1, #original, 2 do
        new[original[i]] = original[i + 1]
    end
    reply[2] = new

    return reply
end

redis_scan.commands.scan = redis_scan.command('SCAN', {        -- >= 2.8----------------------------------------------------------------------------
														request = scan_request,
											})
redis_scan.commands.sscan  = redis_scan.command('SSCAN', {       -- >= 2.8
																request = scan_request,
													})
redis_scan.commands.zscan  = redis_scan.command('ZSCAN', {               -- >= 2.8--------------------------------------------------------------
																request  = scan_request,
																response = zscan_response,
													})
redis_scan.commands.hscan  = redis_scan.command('HSCAN', {       -- >= 2.8-------------------------------------------------------------------------
																request  = scan_request,
																response = hscan_response,
													})
---------------------------------------------------------------------------------------------

return redis_scan