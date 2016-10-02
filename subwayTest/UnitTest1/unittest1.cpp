#include "stdafx.h"
#include "CppUnitTest.h"
#include "../subway_test/city.h"
#include <Windows.h>
#define PATH "C:/Users/jinha/Desktop/subway_test/subway_test/beijing-subway.txt"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
namespace UnitTest1
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		//第一类测试：测试 id2str
		TEST_METHOD(Testd2str_1)
		{
			// TODO: 在此输入测试代码
			City * city = new City(PATH);
			string actual = string(city->id2str(10));
			string expect = string("木樨地");
			Assert::AreEqual(actual, expect);

			actual = string(city->id2str(271));
			expect = string("2号航站楼");
			Assert::AreEqual(actual, expect);

			actual = string(city->id2str(1));
			expect = string("苹果园");
			Assert::AreEqual(actual, expect);
			
			actual = string(city->id2str(197));
			expect = string("西二旗");
			Assert::AreEqual(actual, expect);
		}
		TEST_METHOD(TestId2str_2)
		{
			City * city = new City(PATH);
			char * a = city->id2str(0);
			Assert::AreEqual(NULL, a);

			a = city->id2str(-1);
			Assert::AreEqual(NULL, a);

			a = city->id2str(-100);
			Assert::AreEqual(NULL, a);

			a = city->id2str(272);
			Assert::AreEqual(NULL, a);

			a = city->id2str(100000);
			Assert::AreEqual(NULL, a);
		}
		
		//第二类测试：测试str2id
		TEST_METHOD(TestStr2id_1)
		{
			City * city = new City(PATH);
			Assert::AreEqual(1, city->str2id("苹果园"));
		}
		TEST_METHOD(TestStr2id_2)
		{
			City * city = new City(PATH);
			Assert::AreEqual(-1, city->str2id("奇怪的地方"));
		}
		//第三类测试：测试id2line
		TEST_METHOD(TestId2line_1)
		{
			City * city = new City(PATH);
			Assert::AreEqual("1号线", city->id2line(0));
		}
		TEST_METHOD(TestId2line_2)
		{
			City * city = new City(PATH);
			Assert::AreEqual(NULL, city->id2line(50));
		}
		//第四类测试：测试linked
		TEST_METHOD(TestLinked_1)
		{
			City * city = new City(PATH);
			Assert::AreEqual(1, city->linked(3,4));
			Assert::AreEqual(1, city->linked("上地", "五道口"));
			Assert::AreEqual(1, city->linked("鼓楼大街", "什刹海"));
		}
		TEST_METHOD(TestLinked_2)
		{
			City * city = new City(PATH);
			Assert::AreEqual(0, city->linked(3, 3));
			Assert::AreEqual(0, city->linked(1, 5));
			Assert::AreEqual(0, city->linked("上地", "八道口"));
			Assert::AreEqual(0, city->linked("的地方", "奇奇怪怪"));
		}
		//第五类测试：测试getloc
		TEST_METHOD(TestGetloc_1)
		{
			City * city = new City(PATH);
			Point p = {93, 378};
			Point actual = city->getloc(1);
			Assert::AreEqual(p.x, actual.x);
			Assert::AreEqual(p.y, actual.y);
		}
		TEST_METHOD(TestGetloc_2)
		{
			City * city = new City(PATH);
			Point p = { 0,0 };
			Point actual = city->getloc(-10);
			Assert::AreEqual(p.x, actual.x);
			Assert::AreEqual(p.y, actual.y);

			actual = city->getloc(5000);
			Assert::AreEqual(p.x, actual.x);
			Assert::AreEqual(p.y, actual.y);
		}
		//第六类测试：测试findway
		TEST_METHOD(TestFindway_1)
		{
			City * city = new City(PATH);
			int * line = city->findway("苹果园", "八宝山", 1);
			Assert::AreEqual(line[0], 1);
			Assert::AreEqual(line[1], 2);
			Assert::AreEqual(line[2], 3);
			Assert::AreEqual(line[4], 272);
			free(line);

			line = city->findway("wulukou", "八宝山", 1);
			Assert::AreEqual(line, (int *)NULL);
			free(line);

			line = city->findway("八宝山", "八", 1);
			Assert::AreEqual(line, (int *)NULL);
			free(line);

			line = city->findway("八宝山", "八宝山", 1);
			Assert::AreEqual(line[0], 4);
			Assert::AreEqual(line[1], 272);
			free(line);
		}
		//第七类测试：测试shortestway
		TEST_METHOD(TestShortestway1)
		{
			City * city = new City(PATH);
			int *line = city->shortestway("苹果园", "八宝山");
			Assert::AreEqual(line[0], 1);
			Assert::AreEqual(line[1], 2);
			Assert::AreEqual(line[2], 3);
			Assert::AreEqual(line[4], 272);
			free(line);

			line = city->shortestway("国家图书馆", "菜市口");
			Assert::AreEqual(line[0], 49);
			Assert::AreEqual(line[1], 50);
			Assert::AreEqual(line[2], 39);
			Assert::AreEqual(line[3], -1);
			Assert::AreEqual(line[4], 38);
			free(line);
		}
		//第八类测试：测试least_transfer
		TEST_METHOD(TestLeast_transfer_1)
		{
			City * city = new City(PATH);
			int *line = city->least_transfer("国家图书馆", "菜市口");
			Assert::AreEqual(line[0], 49);
			Assert::AreEqual(line[1], 50);
			Assert::AreEqual(line[2], 39);
			Assert::AreEqual(line[4], 52);
			free(line);
		}
		//第九类测试：测试GetLineNum
		TEST_METHOD(TestGetLineNum_1)
		{
			City * city = new City(PATH);
			Assert::AreEqual(city->getlinenum(), 17);
		}
		//第十类测试：测试GetStaNum
		TEST_METHOD(TestGetStaNum_1)
		{
			City * city = new City(PATH);
			Assert::AreEqual(city->getstanum(), 271);
		}
		//第十一类测试：测试StaToLine
		TEST_METHOD(TestStaToLine_1)
		{
			City * city = new City(PATH);
			Assert::AreEqual(0, city->sta2line(1));
		}
		//第十二类测试：测试GetLine
		TEST_METHOD(TestGetLine_1)
		{
			City * city = new City(PATH);
			int * demo = city->getline(100);
			Assert::AreEqual((int *)NULL, demo);
		}
		TEST_METHOD(TestGetLine_2)
		{
			City * city = new City(PATH);
			int * demo = city->getline(0);
			Assert::AreEqual(demo[0], 1);
		}

	};

}