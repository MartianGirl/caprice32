#include <gtest/gtest.h>

#include "cap32.h"
#include "argparse.h"
#include "keyboard.h"
#include <string>

TEST(ArgParseTest, parseArgsNoArg)
{
   const char *argv[] = {"./cap32"};
   CapriceArgs args;
   std::vector<std::string> slot_list;

   parseArguments(1, const_cast<char **>(argv), slot_list, args);

   ASSERT_EQ(0, slot_list.size());
}

TEST(ArgParseTest, parseArgsOneArg)
{
   const char *argv[] = {"./cap32", "./foo.dsk"};
   CapriceArgs args;
   std::vector<std::string> slot_list;

   parseArguments(2, const_cast<char **>(argv), slot_list, args);

   ASSERT_EQ(1, slot_list.size());
   ASSERT_EQ("./foo.dsk", slot_list.at(0));
}

TEST(ArgParseTest, parseArgsSeveralArgs)
{
   const char *argv[] = {"./cap32", "./foo.dsk", "bar.zip", "0", "__"};
   CapriceArgs args;
   std::vector<std::string> slot_list;

   parseArguments(5, const_cast<char **>(argv), slot_list, args);

   ASSERT_EQ(4, slot_list.size());
   for (int i=1; i < 5; i++)
	   ASSERT_EQ(argv[i], slot_list.at(i-1));
}

TEST(argParseTest, cfgFileArgsSwitch)
{
   const char *argv[] = {"./caprice32", "--cfg_file=/home/caprice32/cap32.cfg"};
   CapriceArgs args;
   std::vector<std::string> slot_list;

   parseArguments(2, const_cast<char **>(argv), slot_list, args);
   ASSERT_EQ("/home/caprice32/cap32.cfg", args.cfgFilePath);
}

TEST(argParseTest, replaceCap32KeysNoKeyword)
{
  std::string command = "print \"Hello, world !\"";

  ASSERT_EQ(command, replaceCap32Keys(command));
}

TEST(argParseTest, replaceCap32KeysKeywords)
{
  std::string command = "print \"Hello, world !\"CAP32_SCRNSHOTCAP32_EXIT";
  std::string expected = "print \"Hello, world !\"\f\x8\f";
  expected += '\0';

  ASSERT_EQ(expected, replaceCap32Keys(command));
}

TEST(argParseTest, replaceCap32KeysRepeatedKeywords)
{
  std::string command = "print \"Hello\"CAP32_SCRNSHOT ; print \",\" ; CAP32_SCRNSHOT ; print \"world !\" ; CAP32_SCRNSHOT";
  std::string expected = "print \"Hello\"\f\x8 ; print \",\" ; \f\x8 ; print \"world !\" ; \f\x8";

  ASSERT_EQ(expected, replaceCap32Keys(command));
}
