from zope.interface import Interface

from twisted.cred.checkers import InMemoryUsernamePasswordDatabaseDontUse
from twisted.trial.unittest import TestCase

from txjsonrpc.auth import HTTPAuthRealm, wrapResource


class HTTPAuthRealmTestCase(TestCase):

    RESOURCE_NAME = 'Test Auth resource'

    def setUp(self):
        self.realm = HTTPAuthRealm(self.RESOURCE_NAME)

    def test_creation(self):
        self.assertEquals(self.realm.resource, self.RESOURCE_NAME)

    def test_logout(self):
        """Logout of the realm causes no errors."""
        # This isn't a great test, but it executes the code.
        self.assertEquals(None, self.realm.logout())

    def test_requestAvatarWeb(self):
        from twisted.web.resource import IResource
        interface, resource, logoutMethod = self.realm.requestAvatar(
            "an id", None, IResource)
        self.assertEquals(interface, IResource)
        self.assertEquals(resource, self.realm.resource)
        self.assertEquals(logoutMethod, self.realm.logout)

    def test_requestAvatarNonWeb(self):
        self.assertRaises(NotImplementedError, self.realm.requestAvatar,
                          "an id", None, [Interface])


class WrapResourceTestCase(TestCase):

    def setUp(self):
        self.checker = InMemoryUsernamePasswordDatabaseDontUse()
        self.checker.addUser("joe", "blow")

    def test_wrapResourceWeb(self):
        from twisted.web.resource import IResource, Resource
        root = Resource()
        wrapped = wrapResource(root, [self.checker])
        self.assertTrue(IResource.providedBy(wrapped))
