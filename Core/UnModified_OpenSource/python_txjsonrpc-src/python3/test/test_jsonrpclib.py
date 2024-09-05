from datetime import datetime
import json

from twisted.trial.unittest import TestCase
from twisted.internet import defer
from txjsonrpc import jsonrpclib
from txjsonrpc.jsonrpclib import (
    Fault, VERSION_PRE1, VERSION_1, VERSION_2, dumps, loads)


class JSONRPCEncoderTestCase(TestCase):
    """Tests for txjsonrpc.jsonrpclib.JSONRPCEncoder."""

    def test_default(self):
        """A date is returned in string format."""
        date = datetime.now()

        encoder = jsonrpclib.JSONRPCEncoder()
        result = encoder.default(date)

        self.assertEquals(date.strftime("%Y%m%dT%H:%M:%S"), result)

    def test_default_not_a_date(self):
        """A non-date param raises TypeError."""
        encoder = jsonrpclib.JSONRPCEncoder()

        self.assertRaises(TypeError, encoder.default, 'string')


class DumpTestCase(TestCase):

    def test_noVersion(self):
        object = {"some": "data"}
        result = dumps(object)
        self.assertEquals(result, '{"some": "data"}')

    def test_noVersionError(self):
        expected = {
            "fault": "Fault",
            "faultCode": "code",
            "faultString": "message"
            }

        fault = Fault("code", "message")
        result = dumps(fault)
        json_result = json.loads(result)

        self.assertEquals(expected, json_result)

    def test_versionPre1(self):
        object = {"some": "data"}
        result = dumps(object, version=VERSION_PRE1)
        self.assertEquals(result, '{"some": "data"}')

    def test_errorVersionPre1(self):
        expected = {
            "fault": "Fault",
            "faultCode": "code",
            "faultString": "message"}

        fault = Fault("code", "message")
        result = dumps(fault, version=VERSION_PRE1)
        json_result = json.loads(result)

        self.assertEquals(expected, json_result)

    def test_version1(self):
        expected = {
            "id": None,
            "result": {"some": "data"},
            "error": None
            }

        data = {"some": "data"}
        result = dumps(data, version=VERSION_1)
        json_result = json.loads(result)

        self.assertEquals(expected, json_result)

    def test_unknown_version(self):
        """An unknown version acts the same as JSONRPC 1.0."""
        expected = {
            "id": None,
            "result": {"some": "data"},
            "error": None
            }

        data = {"some": "data"}
        result = dumps(data, version="JSON-RPC 95")
        json_result = json.loads(result)

        self.assertEquals(expected, json_result)

    def test_errorVersion1(self):
        expected = {
            "id": None,
            "result": None,
            "error": {
                "fault": "Fault",
                "faultCode": "code", "faultString": "message"}
            }

        fault = Fault("code", "message")
        result = dumps(fault, version=VERSION_1)
        json_result = json.loads(result)

        self.assertEquals(expected, json_result)

    def test_version2(self):
        expected = {"jsonrpc": "2.0", "result": {"some": "data"}, "id": None}

        data = {"some": "data"}
        result = dumps(data, version=VERSION_2)
        json_result = json.loads(result)

        self.assertEquals(expected, json_result)

    def test_errorVersion2(self):
        expected = {
            'jsonrpc': '2.0',
            'id': None,
            'error': {
                'message': 'message',
                'code': 'code',
                'data': 'message'
                }
            }

        fault = Fault('code', 'message')
        result = dumps(fault, version=VERSION_2)
        json_result = json.loads(result)

        self.assertEquals(expected, json_result)


class LoadsTestCase(TestCase):

    def test_loads(self):
        jsonInput = ["1", '"a"', '{"apple": 2}', '[1, 2, "a", "b"]']
        expectedResults = [1, "a", {"apple": 2}, [1, 2, "a", "b"]]
        for input, expected in zip(jsonInput, expectedResults):
            unmarshalled = loads(input)
            self.assertEquals(unmarshalled, expected)

    def test_FaultLoads(self):
        dl = []
        for version in (VERSION_PRE1, VERSION_2, VERSION_1):
            object = Fault("code", "message")
            d = defer.maybeDeferred(loads, dumps(object, version=version))
            d = self.assertFailure(d, Fault)

            def callback(exc):
                self.assertEquals(exc.faultCode, object.faultCode)
                self.assertEquals(exc.faultString, object.faultString)
            d.addCallback(callback)

            dl.append(d)
        return defer.DeferredList(dl, fireOnOneErrback=True)


class SimpleParserTestCase(TestCase):
    """Tests for txjsonrpc.jsonrpclib.SimpleParser."""

    def test_feed(self):
        """Data is appended to the parser buffer."""
        parser = jsonrpclib.SimpleParser()
        parser.buffer = 'abc'

        parser.feed('def')

        self.assertEqual('abcdef', parser.buffer)

    def test_close(self):
        expected = {'test': 'close'}
        parser = jsonrpclib.SimpleParser()
        parser.buffer = json.dumps(expected)

        parser.close()

        self.assertEquals(expected, parser.data)


class SimpleUnmarshallerTestCase(TestCase):
    """Tests for txjsonrpc.jsonrpclib.SimpleUnmarshaller."""

    def setUp(self):
        self.unmarshaller = jsonrpclib.SimpleUnmarshaller()
        self.unmarshaller.parser = jsonrpclib.SimpleParser()
        self.unmarshaller.parser.data = {
            'id': 1000,
            'method': 'getMethodName',
            'params': ['a', 'b', 'c'],
            }

    def test_getmethodname(self):
        """The name of the method is returned."""
        name = self.unmarshaller.getmethodname()

        self.assertEqual('getMethodName', name)

    def test_getid(self):
        """The id of the data is returned."""
        id = self.unmarshaller.getid()

        self.assertEqual(1000, id)

    def test_close(self):
        """Params are returned when parser data is a dict."""
        data = self.unmarshaller.close()

        self.assertEqual(['a', 'b', 'c'], data)

    def test_close_raw_data(self):
        """Raw data is returned when parser data is not a dict."""
        self.unmarshaller.parser.data = 'Raw test data'

        data = self.unmarshaller.close()

        self.assertEqual('Raw test data', data)


class GetParserTestCase(TestCase):
    """Tests for txjsonrpc.jsonrpclib.getparser"""

    def test_getparser(self):
        """A parser and marshaller are returned."""
        parser, unmarshaller = jsonrpclib.getparser()

        self.assertTrue(isinstance(parser, jsonrpclib.SimpleParser))
        self.assertTrue(isinstance(unmarshaller, jsonrpclib.SimpleUnmarshaller))
        self.assertIs(parser, unmarshaller.parser)


class TransportTestCase(TestCase):
    """Tests for txjsonrpc.jsonrpclib.Transport."""

    def test_getparser(self):
        """A parser and marshaller are returned."""
        transport = jsonrpclib.Transport()
        parser, unmarshaller = transport.getparser()

        self.assertTrue(isinstance(parser, jsonrpclib.SimpleParser))
        self.assertTrue(isinstance(unmarshaller, jsonrpclib.SimpleUnmarshaller))
        self.assertIs(parser, unmarshaller.parser)
