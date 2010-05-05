#include <tests.h>

#include <stdexcept>

void test_keynew()
{
	cout << "testing keynew" << endl;

	char array[] = "here is some data stored";
	char * getBack;

	Key key0;
	succeed_if(!key0.needSync(), "key1 need sync");
	succeed_if(!key0.needRemove(), "key1 need remove");
	succeed_if( key0.getName() == "", "key0 has wrong name");

	// Empty key
	Key key1 ("", KEY_END);
	succeed_if(!key1.needSync(), "key1 need sync");
	succeed_if(!key1.needRemove(), "key1 need remove");
	succeed_if( key1.getName() == "", "key0 has wrong name");

	// Key with name
	Key key2 ("system/sw/test", KEY_END);
	succeed_if( key2.needSync(), "key2 does not need sync");
	succeed_if(!key2.needRemove(), "key2 need remove");
	succeed_if (key2.getBaseName() == "test", "wrong base name");
	succeed_if( key2.getName() == "system/sw/test", "key2 has wrong name");
	key2.copy(key0);
	succeed_if( key2.getName() == "", "key0 has wrong name");
	succeed_if (key2.getBaseName() == "", "wrong base name");

	// Key with name
	Key key3 ("system/sw/test", KEY_REMOVE, KEY_END);
	succeed_if(key3.needRemove(), "KEY_REMOVE not set");
	succeed_if(key3.getName() == "system/sw/test", "key3 has wrong name");
	succeed_if(key3.getBaseName() == "test", "wrong base name");
	key3 = "system/other/name";
	succeed_if(key3.getName() == "system/other/name", "key3 has wrong name");
	succeed_if(key3.getBaseName() == "name", "wrong base name");
	key3 += "base";
	succeed_if(key3.getName() == "system/other/name/base", "key3 has wrong name");
	succeed_if(key3.getBaseName() == "base", "wrong base name");
	key3 -= "name";
	succeed_if(key3.getName() == "system/other/name/name", "key3 has wrong name");
	succeed_if(key3.getBaseName() == "name", "wrong base name");

	// Key with name + value
	Key key4 ("system/sw/test",
			KEY_VALUE, "test",
			KEY_END);
	succeed_if(key4.getName() == "system/sw/test", "key4 has wrong name");
	succeed_if(key4.get<string>() == "test", "key4 has wrong name");
	// key4.generate(stdout, 0); cout << endl;

	// Key with name + UID/GID
	Key key5 ("system/sw/test",
			KEY_UID, 123,
			KEY_GID, 456,
			KEY_END);
	succeed_if(key5.getUID() == 123, "key5 UID no set correctly");
	succeed_if(key5.getGID() == 456, "key5 GID not set correctly");
	succeed_if(key5.getName() == "system/sw/test", "key5 has wrong name");
	// key5.output(stdout, 0);

	// Key with name + MODE
	Key key6 ("system/sw/test",
			KEY_MODE, 0642,
			KEY_END);
	succeed_if(key6.getMode() == 0642, "key6 mode no set correctly");
	succeed_if(key6.getName() == "system/sw/test", "key6 has wrong name");

	// Key with name + owner
	Key key7 ("system/sw/test",
			KEY_OWNER, "yl",
			KEY_END);
	succeed_if( key7.getOwner() ==  "yl", "key7 owner not set correctly");
	succeed_if (!key7.isInactive(), "key should not be inactive");

	Key key8 ("system/valid/there",
			KEY_BINARY,
			KEY_SIZE, sizeof(array),
			KEY_VALUE, array,
			KEY_END);
	succeed_if(key8.getName() == "system/valid/there", "key8 has wrong name");
	succeed_if(key8.isBinary (), "Key should be binary");
	succeed_if(key8.getValueSize() == sizeof(array), "Value size not correct");
	getBack = new char [key8.getValueSize()];
	key8.getBinary(getBack, key8.getValueSize());
	succeed_if(memcmp(getBack, array, sizeof(array)) == 0, "could not get correct value with keyGetBinary");
	delete [] getBack;
	succeed_if (key8.getBaseName() == "there", "wrong base name");

	Key key9("system/valid/.inactive", KEY_COMMENT, "inactive key", KEY_END);
	succeed_if (key9.isInactive(), "key should be inactive");
	succeed_if (key9.getComment() == "inactive key", "comment failed");
	succeed_if (key9.getBaseName() == ".inactive", "wrong base name");
}

void test_constructor()
{
	cout << "testing constructor" << endl;

	ckdb::Key *ck = ckdb::keyNew(0);
	Key k = ck; // constructor with (ckdb::Key)

	cout << "ck:   " << (void*)ck << endl;
	cout << "k:    " << (void*)&k << endl;
	cout << "k.ck: " << (void*)k.getKey() << endl;

	k.set<int>(30);
	succeed_if (k.get<int> () == 30, "could not get same int");
}

void test_setkey()
{
	cout << "testing setkey" << endl;

	ckdb::Key *ck;
	Key k;

	ck = ckdb::keyNew(0);
	k = ck; // operator= alias for setKey()

	cout << "ck:   " << (void*)ck << endl;
	cout << "k:    " << (void*)&k << endl;
	cout << "k.ck: " << (void*)k.getKey() << endl;

	k.set<int>(30);
	succeed_if (k.get<int> () == 30, "could not get same int");
}

void test_cast()
{
	cout << "testing cast" << endl;
	ckdb::Key *ck;
	Key *k;

	ck = ckdb::keyNew(0);
	k = (Key*) &ck; // no copy, just a cast

	cout << "&ck:  " << (void*)&ck << endl;
	cout << "k:    " << (void*)&k << endl;
	cout << "ck:   " << (void*)ck << endl;
	cout << "k.ck: " << (void*)k->getKey() << endl;

	k->set<int>(30);
	succeed_if (k->get<int> () == 30, "could not get same int");

	ckdb::keyDel (ck);
}

void test_value ()
{
	cout << "testing value" << endl;
	Key test;

	test.setString ("23.3");
	succeed_if (test.get<double> () == 23.3, "could not get same double");
	succeed_if (test.getValueSize () == 5, "value size not correct");

	test.setString ("401");
	succeed_if (test.get<int> () == 401, "could not get same int");
	succeed_if (test.getValueSize () == 4, "value size not correct");

	test.setString ("mystr");
	succeed_if (test.get<string> () == "mystr", "could not get same string");
	succeed_if (test.getValueSize () == 6, "value size not correct");

	test.setString ("myoth");
	succeed_if (test.get<string> () == "myoth", "could not get same string");
	succeed_if (test.getValueSize () == 6, "value size not correct");

	test.set<double> (23.3);
	succeed_if (test.getString() == "23.3", "could not get same double");
	succeed_if (test.getValueSize () == 5, "value size not correct");

	test.set<int> (401);
	succeed_if (test.getString() == "401", "could not get same int");
	succeed_if (test.getValueSize () == 4, "value size not correct");

	test.set<string> ("mystr");
	succeed_if (test.getString() == "mystr", "could not get same string");
	succeed_if (test.getValueSize () == 6, "value size not correct");

	test.set<string> ("myoth");
	succeed_if (test.getString () == "myoth", "could not get same string");
	succeed_if (test.getValueSize () == 6, "value size not correct");

	test.setComment ("mycomment");
	succeed_if (test.getComment () == "mycomment", "could not get same comment");
	succeed_if (test.getCommentSize () == 10, "comment size not correct");
}

void test_exceptions ()
{
	cout << "testing exceptions" << endl;
	Key test;

	try {
		test.setName("no");
	} catch (kdb::KeyInvalidName)
	{
		succeed_if (test.getName() == "", "not set to noname");
	}

	test.setName ("user/name");
	succeed_if (test.getName() == "user/name", "could not get same name");

	try {
		test.setName("no");
	} catch (kdb::KeyInvalidName)
	{
		succeed_if (test.getName() == "", "not set to noname");
	}
	
}

void test_name()
{
	cout << "testing name" << endl;

	Key test;
	test.setName("user:markus/test");
	succeed_if (std::string(test.name()) == "user/test", "Wrong name");
	succeed_if (test.getName() == "user/test", "Wrong name");
	succeed_if (test.getFullName() == "user:markus/test", "Wrong full name");
	succeed_if (std::string(test.owner()) == "markus", "Wrong owner");
	succeed_if (test.getOwner() == "markus", "Wrong owner");
	succeed_if (test.getNameSize() == 10, "wrong name size");
	succeed_if (test.getFullNameSize() == 17, "wrong full name size");
	succeed_if (!test.isSystem(), "key is system");
	succeed_if ( test.isUser(), "key is not user");

	test.setOwner("gerald");
	succeed_if (std::string(test.name()) == "user/test", "Wrong name");
	succeed_if (test.getName() == "user/test", "Wrong name");
	succeed_if (test.getFullName() == "user:gerald/test", "Wrong full name");
	succeed_if (test.getNameSize() == 10, "wrong name size");
	succeed_if (test.getFullNameSize() == 17, "wrong full name size");
	succeed_if (!test.isSystem(), "key is system");
	succeed_if ( test.isUser(), "key is not user");

	test.setName("system/test");
	test.setOwner("markus"); // has no semantics...
	succeed_if (std::string(test.name()) == "system/test", "Wrong name");
	succeed_if (test.getName() == "system/test", "Wrong name");
	succeed_if (test.getFullName() == "system/test", "Wrong full name");
	succeed_if (std::string(test.owner()) == "markus", "Wrong owner");
	succeed_if (test.getOwner() == "markus", "Wrong owner");
	succeed_if (test.getNameSize() == 12, "wrong name size");
	succeed_if (test.getFullNameSize() == 12, "wrong full name size");
	succeed_if ( test.isSystem(), "key is system");
	succeed_if (!test.isUser(), "key is not user");

	test.setName("user/dir/test");
	test.setBaseName ("mykey");
	succeed_if (test.getName() == "user/dir/mykey", "Basename did not work");
	test.setName (test.getName() + "/onedeeper"); // add basename is trivial
	succeed_if (test.getName() == "user/dir/mykey/onedeeper", "Basename did not work");
	succeed_if (test.getName().find('/') == 4, "user length"); // keyGetRootNameSize trivial
	succeed_if (test.isBelow (Key("user/dir/mykey/onedeeper/below", KEY_END)), "key is below");
	succeed_if (!test.isBelow (Key("user/dir/mykey/twodeeper/below", KEY_END)), "key is below");
	succeed_if (test.isDirectBelow (Key("user/dir/mykey/onedeeper/below", KEY_END)), "key is direct below");
	succeed_if (!test.isDirectBelow (Key("user/dir/mykey/onedeeper/twodeeper/below", KEY_END)), "key is direct below");
	succeed_if (!test.isDirectBelow (Key("user/dir/mykey/twodeeper/below", KEY_END)), "key is direct below");
}

void f(Key)
{
	Key h ("user/infunction", KEY_END);
}

void test_ref()
{
	cout << "testing ref" << endl;

	Key zgr1 ("user/zgr1", KEY_END);
	{
		Key zgr2 ("user/zgr2", KEY_END);
		Key zgr3 ("user/zgr3", KEY_END);
		Key zgr4 ("user/zgr4", KEY_END);
		Key zgr5 ("user/zgr5", KEY_END);
		zgr2=zgr1;
		zgr3=zgr1;
		zgr4=zgr1;
	}

	f(zgr1);
	f(Key ("user/passed", KEY_END));

	Key test;
	test.setName("user:markus/test");

	Key ref1;
	ref1 = test; // operator =

	succeed_if (test.getName() == "user/test", "wrong name");
	succeed_if (ref1.getName() == "user/test", "ref key wrong name");

	Key ref2 = test; // copy constructor

	succeed_if (test.getName() == "user/test", "wrong name");
	succeed_if (ref2.getName() == "user/test", "ref key wrong name");

	const Key consttest ("user/test", KEY_END);
	Key ref3 = consttest; // const copy constructor

	succeed_if (consttest.getName() == "user/test", "wrong name");
	succeed_if (ref3.getName() == "user/test", "ref key wrong name");
}

void test_dup()
{
	cout << "testing dup" << endl;

	Key test;
	test.setName("user:markus/test");

	Key dup0 = test.dup(); // directly call of dup()

	succeed_if (test.getName() == "user/test", "wrong name");
	succeed_if (dup0.getName() == "user/test", "dup key wrong name");
}

void test_meta()
{
	cout << "testing metainfo" << endl;
	Key test;

	test.setUID(50);
	succeed_if (test.getUID() == 50, "could not set UID");
	succeed_if (test.getMeta<uid_t>("uid") == 50, "could not set UID");

	test.setMeta<uid_t>("uid", 80);
	succeed_if (test.getUID() == 80, "could not set UID");
	succeed_if (test.getMeta<uid_t>("uid") == 80, "could not set UID");

	test.setGID(50);
	succeed_if (test.getGID() == 50, "could not set GID");
	succeed_if (test.getMeta<gid_t>("gid") == 50, "could not set GID");

	succeed_if (test.getMode() == 0664, "not correct default mode");
	try {
		test.getMeta<mode_t>("mode");
		succeed_if (0, "failed, should raise exception");
	} catch (std::logic_error const &e) {
		succeed_if (1, "exception raised successfully");
	}
	test.setDir ();
	succeed_if (test.isDir(), "is not dir");
	succeed_if (test.getMode() == 0775, "not correct default mode for dir");

	//octal problem for mode:
	succeed_if (test.getMeta<mode_t>("mode") == 775, "not correct default mode for dir");

	test.setMTime (200);
	succeed_if (test.getMTime() == 200, "could not set MTime");
	succeed_if (test.getMeta<time_t>("mtime") == 200, "could not set mtime");

	test.setATime (200);
	succeed_if (test.getATime() == 200, "could not set ATime");
	succeed_if (test.getMeta<time_t>("atime") == 200, "could not set atime");

	test.setCTime (200);
	succeed_if (test.getCTime() == 200, "could not set CTime");
	succeed_if (test.getMeta<time_t>("ctime") == 200, "could not set ctime");

	test.setMeta<int>("myint", 333);
	succeed_if (test.getMeta<int>("myint") == 333, "could not set other meta");

	test.setMeta<double>("mydouble", 333.3);
	succeed_if (test.getMeta<double>("mydouble") == 333.3, "could not set other meta");

	test.setMeta<std::string>("mystr", "str");
	succeed_if (test.getMeta<std::string>("mystr") == "str", "could not set other meta");

	const ckdb::Key *cmeta = test.getMeta<const ckdb::Key*>("mystr");
	succeed_if (!strcmp((const char*)ckdb::keyValue(cmeta), "str"), "could not set other meta");

	const Key meta = test.getMeta<const Key>("mystr");
	succeed_if (meta.getString() == "str", "could not set other meta");
}

void test_iter()
{
	cout << "testing iterating" << endl;

	Key k ("user/metakey",
		KEY_META, "a", "meta",
		KEY_META, "b", "my",
		KEY_META, "c", "other",
		KEY_END);
	Key meta;
	Key end;

	/*
	k.rewindMeta();
	while (!((meta = k.nextMeta()) == end))
	{
		cout << meta.getName() << " " << meta.getString() << endl;
	}
	*/
}

int main()
{
	cout << "KEY CLASS TESTS" << endl;
	cout << "===============" << endl << endl;

	/*
	test_constructor();
	test_setkey();
	test_cast();
	*/

	test_keynew();
	test_name();
	test_value();
	test_exceptions();
	test_dup();
	test_ref();
	test_meta();
	test_iter();

	cout << endl;
	cout << "test_key RESULTS: " << nbTest << " test(s) done. " << nbError << " error(s)." << endl;
	return nbError;
}