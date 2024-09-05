import json

from twisted.internet import defer
from twisted.trial.unittest import TestCase

from txjsonrpc import jsonrpc
from txjsonrpc.jsonrpc import BaseProxy, BaseQueryFactory
from txjsonrpc.jsonrpclib import Fault, VERSION_PRE1, VERSION_1, VERSION_2


class BaseSubhandlerTestCase(TestCase):
    """Tests for txjsonrpc.jsonrpc.BaseSubhandler."""

    def setUp(self):
        self.subhandler = jsonrpc.BaseSubhandler()

    def test_init(self):
        """An empty dict of subhandlers is added."""
        self.assertEqual({}, self.subhandler.subHandlers)

    def test_putSubHandler(self):
        """A subhandler is added."""
        self.subhandler.putSubHandler('test', 'foo-bar-baz')

        self.assertEqual(
            'foo-bar-baz', self.subhandler.subHandlers['test'])

    def test_getSubHandler(self):
        """A subhandler is retrieved."""
        self.subhandler.subHandlers['test'] = 'foo-bar-baz'

        result = self.subhandler.getSubHandler('test')

        self.assertEqual('foo-bar-baz', result)

    def test_getSubHandlerPrefixes(self):
        """A list of subhandlers prefixes is returned."""
        self.subhandler.subHandlers['test-a'] = 'foo-bar-baz'
        self.subhandler.subHandlers['test-b'] = 'foo-bar-baz'
        self.subhandler.subHandlers['test-c'] = 'foo-bar-baz'

        result = self.subhandler.getSubHandlerPrefixes()

        self.assertEqual(
            ['test-a', 'test-b', 'test-c'], sorted(result))


class BaseQueryFactoryTestCase(TestCase):

    def test_creation(self):
        """Properties are set on the factory on creation."""
        factory = BaseQueryFactory("someMethod")
        payload = json.loads(factory.payload)

        self.assertEqual(VERSION_PRE1, factory.version)
        self.assertEqual(1, factory.id)
        self.assertTrue(isinstance(factory.deferred, defer.Deferred))
        self.assertEqual(
            {'method': 'someMethod', 'params': []}, payload)

    def test_buildVersionedPayloadPre1(self):
        expected = {'params': [], 'method': 'someMethod'}

        factory = BaseQueryFactory("someMethod", version=VERSION_PRE1)
        json_payload = json.loads(factory.payload)

        self.assertEquals(expected, json_payload)

    def test_buildVersionedPayload1(self):
        expected = {
            'params': [],
            'method': 'someMethod',
            'id': 1
            }

        factory = BaseQueryFactory("someMethod", version=VERSION_1)
        json_payload = json.loads(factory.payload)

        self.assertEquals(expected, json_payload)

    def test_buildVersionedPayload2(self):
        expected = {
            'params': [],
            'jsonrpc': '2.0',
            'method': 'someMethod',
            'id': 1
            }

        factory = BaseQueryFactory("someMethod", version=VERSION_2)
        json_payload = json.loads(factory.payload)

        self.assertEquals(expected, json_payload)

    def test_parseResponseNoJSON(self):
        factory = BaseQueryFactory("someMethod")
        d = factory.deferred
        factory.parseResponse("oops")
        self.assertFailure(d, ValueError)
        return d

    def test_parseResponseRandomJSON(self):
        expected = {'something': 1}

        factory = BaseQueryFactory("someMethod")
        d = factory.deferred
        factory.parseResponse(json.dumps({'something': 1}))
        return d.addCallback(
            lambda result: self.assertEquals(expected, result))

    def test_parseResponseFaultData(self):

        def check_error(error):
            self.assertTrue(isinstance(error.value, Fault))
            self.assertEquals(error.value.faultCode, 1)
            self.assertEquals(error.value.faultString, u"oops")

        factory = BaseQueryFactory("someMethod")
        d = factory.deferred
        factory.parseResponse(json.dumps(
            {"fault": "Fault", "faultCode": 1, "faultString": "oops"}))
        return d.addErrback(check_error)


class BaseProxyTestCase(TestCase):

    def test_creation(self):
        proxy = BaseProxy()
        self.assertEquals(proxy.version, VERSION_PRE1)
        self.assertEquals(proxy.factoryClass, None)

    def test_getVersionDefault(self):
        proxy = BaseProxy()
        version = proxy._getVersion({})
        self.assertEquals(version, VERSION_PRE1)

    def test_getVersionPre1(self):
        proxy = BaseProxy()
        version = proxy._getVersion({"version": VERSION_PRE1})
        self.assertEquals(version, VERSION_PRE1)

    def test_getVersion1(self):
        proxy = BaseProxy()
        version = proxy._getVersion({"version": VERSION_1})
        self.assertEquals(version, VERSION_1)

    def test_getFactoryClassDefault(self):
        proxy = BaseProxy()
        factoryClass = proxy._getFactoryClass({})
        self.assertEquals(factoryClass, None)

    def test_getFactoryClassPassed(self):

        class FakeFactory(object):
            pass

        proxy = BaseProxy()
        factoryClass = proxy._getFactoryClass({"factoryClass": FakeFactory})
        self.assertEquals(factoryClass, FakeFactory)


class FakeHandler(jsonrpc.BaseSubhandler):
    """A Fake Handler for Introspection tests."""

    def jsonrpc_test(self, paramA, paramB):
        """A test method that does nothing."""
    jsonrpc_test.signature = [None, str, int]


class IntrospectionTestCase(TestCase):
    """Tests for txjsonrpc.jsonrpc.Introspection."""

    def test_init(self):
        """Initial properties are set on creation."""
        parent = FakeHandler()
        api = jsonrpc.Introspection(parent)

        self.assertEqual({}, api.subHandlers)  # From BaseSubHandler
        self.assertEqual(parent, api._jsonrpc_parent)

    def test_jsonrpc_listMethods(self):
        """A list of supported method names is returned."""
        api = jsonrpc.Introspection(FakeHandler())

        methods = api.jsonrpc_listMethods()

        self.assertEqual(['test'], methods)

    def test_jsonrpc_methodHelp(self):
        """A docstring is returned."""
        api = jsonrpc.Introspection(FakeHandler())

        doc = api.jsonrpc_methodHelp('test')

        self.assertEqual(
            'A test method that does nothing.', doc)

    def test_jsonrpc_methodSignature(self):
        """The function signature is returned."""
        api = jsonrpc.Introspection(FakeHandler())

        doc = api.jsonrpc_methodSignature('test')

        self.assertEqual(
            [None, str, int], doc)


class AddIntrospectionTestCase(TestCase):
    """Tests for txjsonrpc.jsonrpc.addIntrospection."""

    def test_addIntrospection(self):
        """A 'system' handler is installed."""
        handler = FakeHandler()

        jsonrpc.addIntrospection(handler)

        self.assertIn('system', handler.subHandlers)
