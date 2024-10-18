/* speed - Generic C++ library.
 * Copyright (C) 2015-2024 Killian Valverde.
 *
 * This file is part of speed.
 *
 * speed is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * speed is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with speed. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file        speed_gtest/argparse_test/arg_parser_test.cpp
 * @brief       arg_parser unit test.
 * @author      Killian Valverde
 * @date        2017/05/16
 */

#include <gtest/gtest.h>

#include "speed/argparse/argparse.hpp"


class argparse_arg_parser : public ::testing::Test
{
protected:
    argparse_arg_parser()
    {
    }

    void SetUp() override
    {
        ap.configure()
                .colors(false)
                .print_errors(false)
                .print_help_after_printing_errors(false)
                .pkill_after_printing_errors(false);
    }

    speed::argparse::arg_parser ap;
};


TEST_F(argparse_arg_parser, configure)
{
    ap.configure()
            .colors(false)
            .error_id("error")
            .long_prefixes("--")
            .maximum_unrecognized_args(16)
            .pkill_after_printing_errors(true)
            .print_errors(true)
            .print_help_after_printing_errors(false)
            .program_name("speed")
            .short_prefixes("-")
            .short_prefixes("-");
}


TEST_F(argparse_arg_parser, add_key_arg)
{
    bool presnc;

    ap.add_key_arg("-l", "--long")
            .action([&]() {})
            .description("Recursive behavior.")
            .error_name("error")
            .grouping(true)
            .help_menus_assigned("help1", "help2")
            .mandatory(false)
            .store_presence(&presnc)
            .terminal(false)
            .unique_instance(true)
            .unique_instance(true);

    EXPECT_THROW(ap.add_key_arg(), speed::argparse::exception);
    EXPECT_THROW(ap.add_key_arg("-l"), speed::argparse::exception);
    EXPECT_THROW(ap.add_key_arg("--long"), speed::argparse::exception);
    EXPECT_TRUE(!ap.was_found("-l"));
}


TEST_F(argparse_arg_parser, add_key_value_arg)
{
    std::vector<std::string> vals;
    std::uint64_t holdr1 = 0;
    double holdr2 = 0;
    bool presnc;

    ap.add_key_value_arg("-s", "--seconds")
            .action([&]() {})
            .assignment_operator(true)
            .description("The number of seconds.")
            .error_name("error")
            .grouping(true)
            .help_menus_assigned("help1", "help2")
            .mandatory(false)
            .minmax_values(1, 1)
            .store_into(&vals)
            .regexes("^.*$")
            .store_into(&holdr1, &holdr2)
            .store_presence(&presnc)
            .terminal(false)
            .unique_instance(true)
            .values_names("VALUE")
            .values_with_prefix(false)
            .values_with_prefix(false);

    EXPECT_THROW(ap.add_key_value_arg(), speed::argparse::exception);
    EXPECT_THROW(ap.add_key_value_arg("-s"), speed::argparse::exception);
    EXPECT_THROW(ap.add_key_value_arg("--seconds"), speed::argparse::exception);
    EXPECT_TRUE(!ap.was_found("-s"));
}


TEST_F(argparse_arg_parser, add_keyless_arg)
{
    std::vector<std::string> vals;
    std::uint64_t holdr1 = 0;
    double holdr2 = 0;
    bool presnc;

    ap.add_keyless_arg("FILE")
            .action([&]() {})
            .description("The file path.")
            .error_name("error")
            .help_menus_assigned("help1", "help2")
            .mandatory(true)
            .minmax_values(1, 1)
            .store_into(&vals)
            .regexes("^.*$")
            .store_into(&holdr1, &holdr2)
            .store_presence(&presnc)
            .terminal(false)
            .unique_instance(true)
            .values_with_prefix(false)
            .values_with_prefix(false);

    EXPECT_THROW(ap.add_keyless_arg(""), speed::argparse::exception);
    EXPECT_THROW(ap.add_keyless_arg("FILE"), speed::argparse::exception);
    EXPECT_TRUE(!ap.was_found("FILE"));
}


TEST_F(argparse_arg_parser, add_help_arg)
{
    std::vector<std::string> vals;
    std::uint64_t holdr1 = 0;
    double holdr2 = 0;
    bool presnc;

    ap.add_help_arg("-h", "--help")
            .action([&]() {})
            .assignment_operator(false)
            .description("Display this help and exit.")
            .error_name("error")
            .grouping(false)
            .help_menus_assigned("help1", "help2")
            .help_menus_triggered("^.*$", "help1")
            .mandatory(false)
            .minmax_values(0, 0)
            .pkill_after_triggering(true)
            .store_into(&vals)
            .regexes("^.*$")
            .store_into(&holdr1, &holdr2)
            .store_presence(&presnc)
            .terminal(false)
            .trigger_help_printing(false)
            .unique_instance(true)
            .values_names("VALUE")
            .values_with_prefix(false)
            .values_with_prefix(false);

    EXPECT_THROW(ap.add_help_arg(), speed::argparse::exception);
    EXPECT_THROW(ap.add_help_arg("-h"), speed::argparse::exception);
    EXPECT_THROW(ap.add_help_arg("--help"), speed::argparse::exception);
    EXPECT_TRUE(!ap.was_found("-h"));
}


TEST_F(argparse_arg_parser, add_version_arg)
{
    bool presnc;

    ap.add_version_arg("-v", "--version")
            .action([&]() {})
            .description("Display version information.")
            .error_name("error")
            .gplv3_version_information("v1.0.1", "2024", "Killian Valverde")
            .grouping(true)
            .help_menus_assigned("help1", "help2")
            .mandatory(false)
            .pkill_after_triggering(false)
            .store_presence(&presnc)
            .terminal(false)
            .trigger_version_printing(false)
            .unique_instance(true)
            .version_information("v1.0.0")
            .version_information("v1.0.0");

    EXPECT_THROW(ap.add_version_arg(), speed::argparse::exception);
    EXPECT_THROW(ap.add_version_arg("-v"), speed::argparse::exception);
    EXPECT_THROW(ap.add_version_arg("--version"), speed::argparse::exception);
    EXPECT_TRUE(!ap.was_found("-v"));
}


TEST_F(argparse_arg_parser, add_help_menu)
{
    ap.add_help_menu()
            .description("Description.");

    ap.add_help_arg("--help", "-h")
            .description("Display this help and exit.");

    ap.add_version_arg("--version", "-v")
            .description("Output version information and exit")
            .gplv3_version_information("1.0.0", "2024", "Killian");
}


TEST_F(argparse_arg_parser, parse_key_args)
{
    std::vector<const char*> argv = {
        "speed",
        "-a",
        "--recursive"
    };

    bool flg_all;
    bool flg_long;
    bool flg_recursive;
    std::size_t dee = 0;

    ap.add_key_arg("-a", "--all")
            .description("Display all the information.")
            .store_presence(&flg_all);

    ap.add_key_arg("-l", "--long")
            .description("Display the list in a not compacted mode.")
            .store_presence(&flg_long);

    ap.add_key_arg("-r", "--recursive")
            .description("Execute the process in a recursive way.")
            .store_presence(&flg_recursive)
            .action([&]() { ++dee; });

    EXPECT_NO_THROW(ap.parse_args(argv.size(), argv));
    EXPECT_TRUE(flg_all);
    EXPECT_TRUE(!flg_long);
    EXPECT_TRUE(flg_recursive);
    EXPECT_TRUE(ap.was_found("-a"));
    EXPECT_TRUE(!ap.was_found("-l"));
    EXPECT_TRUE(ap.was_found("-r"));
    EXPECT_TRUE(dee == 1);
}


TEST_F(argparse_arg_parser, parse_key_value_args)
{
    std::vector<const char*> argv = {
        "speed",
        "-s", "10", "20",
        "-m", "20", "50", "60",
        "-h", "jkdf"
    };

    std::vector<std::uint64_t> mins_vec;
    std::uint64_t secs = 0;
    std::uint64_t hrs = 0;
    std::uint64_t dm;

    ap.add_key_value_arg("--seconds", "-s")
            .description("Set seconds.")
            .values_names("INTEGER")
            .store_into(&secs);

    ap.add_key_value_arg("--minutes", "-m")
            .description("Set minutes.")
            .values_names("INTEGER")
            .minmax_values(1, 2)
            .store_into(&mins_vec);

    ap.add_key_value_arg("--hours", "-h")
            .description("Set hours.")
            .values_names("INTEGER")
            .store_into(&hrs);

    EXPECT_NO_THROW(ap.parse_args(argv.size(), argv));
    EXPECT_TRUE(ap.count_values_found("-s") == 1);
    EXPECT_TRUE(ap.count_values_found("--minutes") == 2);
    EXPECT_TRUE(ap.count_values_found("-h") == 1);
    EXPECT_TRUE(secs == 10);
    EXPECT_TRUE(mins_vec[0] == 20);
    EXPECT_TRUE(mins_vec[1] == 50);
    EXPECT_TRUE(hrs == 0);
    EXPECT_TRUE(ap.get_front_as<std::uint64_t>("-s") == 10);
    EXPECT_TRUE(ap.get_front_as<std::uint64_t>("-m") == 20);
    EXPECT_TRUE(ap.get_at_as<std::uint64_t>("-m", 1) == 50);
    EXPECT_THROW(dm = ap.get_at_as<std::uint64_t>("-s", 1), speed::argparse::exception);
    EXPECT_THROW(dm = ap.get_at_as<std::uint64_t>("-m", 2), speed::argparse::exception);
    EXPECT_THROW(dm = ap.get_at_as<std::uint64_t>("-m", 3), speed::argparse::exception);
    EXPECT_THROW(dm = ap.get_front_as<std::uint64_t>("-h"), speed::type_casting::exception);
}


TEST_F(argparse_arg_parser, parse_keyless_args)
{
    std::vector<const char *> argv = {
        "speed",
        "/home/user/Desktop",
        "-s", "45",
        "/home/user/Pictures",
        "/home/user/Videos",
        "/home/user/Documents",
        "-n",
        "78",
        "/home/user/Favorites",
        "79"
    };

    std::vector<std::size_t> secs;
    std::vector<std::string> pths1;
    std::vector<std::string> pths2;
    std::vector<std::string> pths3;
    std::vector<std::size_t> numbr1;
    std::vector<std::size_t> numbr2;

    ap.add_key_value_arg("--seconds", "-s")
            .description("Set seconds.")
            .values_names("INTEGER")
            .minmax_values(1, ~0ull)
            .store_into(&secs);

    ap.add_keyless_arg("DESTINATION1")
            .description("Destination directory.")
            .store_into(&pths1);

    ap.add_keyless_arg("DESTINATION2")
            .description("Destination directory.")
            .store_into(&pths2);

    ap.add_keyless_arg("DESTINATION3")
            .description("Destination directory.")
            .store_into(&pths3)
            .minmax_values(1, 2);

    ap.add_keyless_arg("NUMBER1")
            .description("Some number.")
            .store_into(&numbr1)
            .minmax_values(1, 2);

    ap.add_keyless_arg("NUMBER2")
            .description("Some number.")
            .store_into(&numbr2)
            .minmax_values(1, 2);

    EXPECT_NO_THROW(ap.parse_args(argv.size(), argv));
    EXPECT_TRUE(secs[0] == 45);
    EXPECT_TRUE(ap.count_values_found("DESTINATION1") == 1);
    EXPECT_TRUE(pths1[0] == "/home/user/Desktop");
    EXPECT_TRUE(ap.get_front_as<std::string>("DESTINATION1") == "/home/user/Desktop");
    EXPECT_TRUE(ap.count_values_found("DESTINATION2") == 1);
    EXPECT_TRUE(pths2[0] == "/home/user/Pictures");
    EXPECT_TRUE(ap.get_front_as<std::string>("DESTINATION2") == "/home/user/Pictures");
    EXPECT_TRUE(ap.count_values_found("DESTINATION3") == 2);
    EXPECT_TRUE(pths3[0] == "/home/user/Videos");
    EXPECT_TRUE(pths3[1] == "/home/user/Documents");
    EXPECT_TRUE(ap.get_front_as<std::string>("DESTINATION3") == "/home/user/Videos");
    EXPECT_TRUE(ap.count_values_found("NUMBER1") == 1);
    EXPECT_TRUE(numbr1[0] == 78);
    EXPECT_TRUE(ap.get_front_as<std::size_t>("NUMBER1") == 78);
    EXPECT_TRUE(ap.count_values_found("NUMBER2") == 2);
    EXPECT_TRUE(numbr2.size() == 1);
    EXPECT_TRUE(numbr2[0] == 79);
    EXPECT_TRUE(ap.get_front_as<std::string>("NUMBER2") == "/home/user/Favorites");
    EXPECT_TRUE(ap.get_at_as<std::size_t>("NUMBER2", 1) == 79);
    EXPECT_TRUE(ap.has_errors());
}


TEST_F(argparse_arg_parser, parse_help_args)
{
    std::vector<const char*> argv = {
            "speed",
            "-h", "information", "information2",
    };

    std::string help_cat;
    std::string dm;
    bool presnc;

    ap.add_help_arg("--help", "-h")
            .description("Display this help and exit.")
            .values_names("CATEGORY")
            .store_into(&help_cat)
            .store_presence(&presnc)
            .trigger_help_printing(false);

    EXPECT_NO_THROW(ap.parse_args(argv.size(), argv));
    EXPECT_TRUE(presnc);
    EXPECT_TRUE(ap.count_values_found("-h") == 1);
    EXPECT_TRUE(help_cat == "information");
    EXPECT_TRUE(ap.get_front_as<std::string>("-h") == "information");
    EXPECT_THROW(dm = ap.get_at_as<std::string>("-h", 1), speed::argparse::exception);
}


TEST_F(argparse_arg_parser, parse_version_args)
{
    std::vector<const char*> argv = {
            "speed",
            "-v", "information"
    };

    std::string help_cat;
    bool presnc;

    ap.add_version_arg("--version", "-v")
            .description("Display version information.")
            .store_presence(&presnc)
            .trigger_version_printing(false);

    EXPECT_NO_THROW(ap.parse_args(argv.size(), argv));
    EXPECT_TRUE(presnc);
    EXPECT_TRUE(ap.was_found("-v"));
    EXPECT_TRUE(ap.was_found("--version"));
}


TEST_F(argparse_arg_parser, parse_eq_operator)
{
    std::vector<const char*> argv = {
        "speed",
        "-s=10", "20",
        "-m=20", "50", "60",
        "-h=jkdf"
    };

    std::vector<std::uint64_t> mins_vec;
    std::uint64_t secs = 0;
    std::uint64_t hrs = 0;
    std::uint64_t dm;

    ap.add_key_value_arg("--seconds", "-s")
            .description("Set seconds.")
            .values_names("INTEGER")
            .store_into(&secs);

    ap.add_key_value_arg("--minutes", "-m")
            .description("Set minutes.")
            .values_names("INTEGER")
            .minmax_values(2, 2)
            .store_into(&mins_vec);

    ap.add_key_value_arg("--hours", "-h")
            .description("Set hours.")
            .values_names("INTEGER")
            .store_into(&hrs);

    EXPECT_NO_THROW(ap.parse_args(argv.size(), argv));
    EXPECT_TRUE(ap.count_values_found("-s") == 1);
    EXPECT_TRUE(ap.count_values_found("--minutes") == 2);
    EXPECT_TRUE(ap.count_values_found("-h") == 1);
    EXPECT_TRUE(secs == 10);
    EXPECT_TRUE(mins_vec[0] == 20);
    EXPECT_TRUE(mins_vec[1] == 50);
    EXPECT_TRUE(hrs == 0);
    EXPECT_TRUE(ap.get_front_as<std::uint64_t>("-s") == 10);
    EXPECT_TRUE(ap.get_front_as<std::uint64_t>("-m") == 20);
    EXPECT_TRUE(ap.get_at_as<std::uint64_t>("-m", 1) == 50);
    EXPECT_THROW(dm = ap.get_at_as<std::uint64_t>("-s", 1), speed::argparse::exception);
    EXPECT_THROW(dm = ap.get_at_as<std::uint64_t>("-m", 2), speed::argparse::exception);
    EXPECT_THROW(dm = ap.get_at_as<std::uint64_t>("-m", 3), speed::argparse::exception);
    EXPECT_THROW(dm = ap.get_front_as<std::uint64_t>("-h"), speed::type_casting::exception);
}


TEST_F(argparse_arg_parser, parse_grouping)
{
    std::vector<const char*> argv = {
        "speed",
        "-sm", "10", "20", "50", "60",
        "-h", "jkdf"
    };

    std::vector<std::uint64_t> mins_vec;
    std::uint64_t secs = 0;
    std::uint64_t hrs = 0;
    std::uint64_t dm;

    ap.add_key_value_arg("--seconds", "-s")
            .description("Set seconds.")
            .values_names("INTEGER")
            .store_into(&secs);

    ap.add_key_value_arg("--minutes", "-m")
            .description("Set minutes.")
            .values_names("INTEGER")
            .store_into(&mins_vec)
            .minmax_values(2, 2);

    ap.add_key_value_arg("--hours", "-h")
            .description("Set hours.")
            .values_names("INTEGER")
            .store_into(&hrs);

    EXPECT_NO_THROW(ap.parse_args(argv.size(), argv));
    EXPECT_TRUE(ap.count_values_found("-s") == 1);
    EXPECT_TRUE(ap.count_values_found("--minutes") == 2);
    EXPECT_TRUE(ap.count_values_found("-h") == 1);
    EXPECT_TRUE(secs == 10);
    EXPECT_TRUE(mins_vec[0] == 20);
    EXPECT_TRUE(mins_vec[1] == 50);
    EXPECT_TRUE(hrs == 0);
    EXPECT_TRUE(ap.get_front_as<std::uint64_t>("-s") == 10);
    EXPECT_TRUE(ap.get_front_as<std::uint64_t>("-m") == 20);
    EXPECT_TRUE(ap.get_at_as<std::uint64_t>("-m", 1) == 50);
    EXPECT_THROW(dm = ap.get_at_as<std::uint64_t>("-s", 1), speed::argparse::exception);
    EXPECT_THROW(dm = ap.get_at_as<std::uint64_t>("-m", 2), speed::argparse::exception);
    EXPECT_THROW(dm = ap.get_at_as<std::uint64_t>("-m", 3), speed::argparse::exception);
    EXPECT_THROW(dm = ap.get_front_as<std::uint64_t>("-h"), speed::type_casting::exception);
}


TEST_F(argparse_arg_parser, parse_alof_constraint)
{
    std::vector<const char*> argv1 = {
        "speed"
    };

    std::vector<const char*> argv2 = {
        "speed",
        "-a"
    };

    ap.add_key_arg("-a", "--all")
            .description("Display all the information.");

    ap.add_key_arg("-l", "--long")
            .description("Display the list in a not compacted mode.");

    ap.add_key_arg("-r", "--recursive")
            .description("Execute the process in a recursive way.");

    ap.add_at_least_one_found_constraint("-a", "-l", "-r");

    ap.parse_args(argv1.size(), argv1);
    EXPECT_TRUE(ap.has_errors());

    ap.parse_args(argv2.size(), argv2);
    EXPECT_TRUE(!ap.has_errors());
}


TEST_F(argparse_arg_parser, parse_mutually_exclusive_constraint)
{
    std::vector<const char*> argv1 = {
        "speed",
        "-a",
        "-l"
    };

    std::vector<const char*> argv2 = {
        "speed",
        "-a"
    };

    ap.add_key_arg("-a", "--all")
            .description("Display all the information.");

    ap.add_key_arg("-l", "--long")
            .description("Display the list in a not compacted mode.");

    ap.add_key_arg("-r", "--recursive")
            .description("Execute the process in a recursive way.");

    ap.add_mutually_exclusive_constraint("-a", "-l", "-r");

    ap.parse_args(argv1.size(), argv1);
    EXPECT_TRUE(ap.was_found("-a"));
    EXPECT_TRUE(ap.was_found("-l"));
    EXPECT_TRUE(!ap.was_found("-r"));
    EXPECT_TRUE(ap.has_errors());

    ap.parse_args(argv2.size(), argv2);
    EXPECT_TRUE(ap.was_found("-a"));
    EXPECT_TRUE(!ap.was_found("-l"));
    EXPECT_TRUE(!ap.was_found("-r"));
    EXPECT_TRUE(!ap.has_errors());
}


TEST_F(argparse_arg_parser, parse_all_constraints)
{
    std::vector<const char*> argv1 = {
        "speed",
        "-a",
        "-l"
    };

    std::vector<const char*> argv2 = {
        "speed"
    };

    std::vector<const char*> argv3 = {
        "speed",
        "-a"
    };

    ap.add_key_arg("-a", "--all")
            .description("Display all the information.");

    ap.add_key_arg("-l", "--long")
            .description("Display the list in a not compacted mode.");

    ap.add_key_arg("-r", "--recursive")
            .description("Execute the process in a recursive way.");

    ap.add_at_least_one_found_constraint("-a", "-l", "-r");
    ap.add_mutually_exclusive_constraint("-a", "-l", "-r");

    ap.parse_args(argv1.size(), argv1);
    EXPECT_TRUE(ap.was_found("-a"));
    EXPECT_TRUE(ap.was_found("-l"));
    EXPECT_TRUE(!ap.was_found("-r"));
    EXPECT_TRUE(ap.has_errors());

    ap.parse_args(argv2.size(), argv2);
    EXPECT_TRUE(!ap.was_found("-a"));
    EXPECT_TRUE(!ap.was_found("-l"));
    EXPECT_TRUE(!ap.was_found("-r"));
    EXPECT_TRUE(ap.has_errors());

    ap.parse_args(argv3.size(), argv3);
    EXPECT_TRUE(ap.was_found("-a"));
    EXPECT_TRUE(!ap.was_found("-l"));
    EXPECT_TRUE(!ap.was_found("-r"));
    EXPECT_TRUE(!ap.has_errors());
}


TEST_F(argparse_arg_parser, parse_sub_parser)
{
    std::vector<const char*> argv1 = {
        "git",
        "add",
        "src/speed/argparse/basic_arg_parser.hpp"
    };

    std::vector<const char*> argv2 = {
        "git",
        "commit",
        "-m",
        "test: argparse: add test"
    };

    std::vector<const char*> argv3 = {
        "git",
        "rebase",
        "--interactive"
    };

    speed::argparse::arg_parser add_parsr;
    speed::argparse::arg_parser commit_parsr;
    speed::argparse::arg_parser rebase_parsr;
    std::filesystem::path pth;
    std::string messag;
    bool interactv = false;

    add_parsr.add_keyless_arg("FILE")
            .store_into(&pth);

    commit_parsr.add_key_value_arg("-m")
            .store_into(&messag);

    rebase_parsr.add_key_arg("-i", "--interactive")
            .store_presence(&interactv);

    ap.add_key_arg("add")
            .sub_parser(&add_parsr);

    ap.add_key_arg("commit")
            .sub_parser(&commit_parsr);

    ap.add_key_arg("rebase")
            .sub_parser(&rebase_parsr);

    ap.parse_args(argv1.size(), argv1);
    EXPECT_TRUE(pth == argv1[2]);

    ap.parse_args(argv2.size(), argv2);
    EXPECT_TRUE(messag == argv2[3]);

    ap.parse_args(argv3.size(), argv3);
    EXPECT_TRUE(interactv);
}


TEST_F(argparse_arg_parser, check_errors)
{
    std::vector<const char*> argv = {
        "speed",
        "-a",
        "-b",
        "--recursive"
    };

    bool flg_all;
    bool flg_long;
    bool flg_recursive;

    ap.add_key_arg("-a", "--all")
            .description("Display all the information.")
            .store_presence(&flg_all);

    ap.add_key_arg("-l", "--long")
            .description("Display the list in a not compacted mode.")
            .store_presence(&flg_long);

    ap.add_key_arg("-r", "--recursive")
            .description("Execute the process in a recursive way.")
            .store_presence(&flg_recursive);

    EXPECT_NO_THROW(ap.parse_args(argv.size(), argv));
    EXPECT_TRUE(flg_all);
    EXPECT_TRUE(!flg_long);
    EXPECT_TRUE(flg_recursive);
    EXPECT_TRUE(ap.was_found("-a"));
    EXPECT_TRUE(!ap.was_found("-l"));
    EXPECT_TRUE(ap.was_found("-r"));
    EXPECT_TRUE(!ap.arg_has_errors("-a"));
    EXPECT_TRUE(!ap.arg_has_errors("-l"));
    EXPECT_TRUE(!ap.arg_has_errors("-r"));
    EXPECT_TRUE(ap.has_errors());
}


TEST_F(argparse_arg_parser, print_usage)
{
    std::string expected_res =
        "Usage: speed [OPTION]... --seconds=INTEGER --minutes=INTEGER... "
        "--hours[=INTEGER]... DESTINATION\n\n";

    speed::iostream::ios_redirect ios_redirect(std::cout);
    ios_redirect.redirect_to_internal_stream();

    ap.configure()
            .program_name("speed");

    ap.add_help_menu()
            .print_options(false)
            .print_commands(false)
            .print_values(false);

    ap.add_key_arg("-a", "--all")
            .description("Display all the information.");

    ap.add_key_arg("-l", "--long")
            .description("Display the list in a not compacted mode.");

    ap.add_key_arg("-r", "--recursive")
            .description("Execute the process in a recursive way.");

    ap.add_key_value_arg("--seconds", "-s")
            .description("Set seconds.")
            .values_names("INTEGER")
            .mandatory(true);

    ap.add_key_value_arg("--minutes", "-m")
            .description("Set minutes.")
            .values_names("INTEGER")
            .mandatory(true)
            .minmax_values(2, 4);

    ap.add_key_value_arg("--hours", "-h")
            .description("Set hours.")
            .values_names("INTEGER")
            .mandatory(true)
            .minmax_values(0, 4);

    ap.add_keyless_arg("DESTINATION")
            .description("Destination directory.");

    ap.print_help();

    EXPECT_TRUE(ios_redirect.get_internal_string() == expected_res);
}


TEST_F(argparse_arg_parser, print_description)
{
    std::string expected_res =
            "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed ullamcorper metus\n"
            "non nisi mattis, sit amet vestibulum ipsum vulputate. Quisque sollicitudin enim\n"
            "a felis vehicula, quis faucibus mi molestie. Fusce id justo et tortor vehicula\n"
            "porttitor.\n\n";

    speed::iostream::ios_redirect ios_redirect(std::cout);
    ios_redirect.redirect_to_internal_stream();

    ap.configure()
            .program_name("speed");

    ap.add_help_menu()
            .print_usage(false)
            .print_options(false)
            .print_commands(false)
            .print_values(false)
            .description(
                    "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed ullamcorper metus"
                    " non nisi mattis, sit amet vestibulum ipsum vulputate. Quisque sollicitudin "
                    "enim a felis vehicula, quis faucibus mi molestie. Fusce id justo et tortor "
                    "vehicula porttitor.");

    ap.add_key_arg("-a", "--all")
            .description("Display all the information.");

    ap.print_help();

    EXPECT_TRUE(ios_redirect.get_internal_string() == expected_res);
}


TEST_F(argparse_arg_parser, print_options)
{
    std::string expected_res =
        "Options:\n"
        "  -a, --all                 Display all the information.\n"
        "  -r, --recursive           Execute the process in a recursive way.\n"
        "  -h, --hours[=INTEGER]...  Set hours.\n\n";

    speed::iostream::ios_redirect ios_redirect(std::cout);
    ios_redirect.redirect_to_internal_stream();

    ap.configure()
            .program_name("speed");

    ap.add_help_menu()
            .print_usage(false)
            .print_commands(false)
            .print_values(false);

    ap.add_key_arg("-a", "--all")
            .description("Display all the information.");

    ap.add_key_arg("-l", "--long")
            .description("Display the list in a not compacted mode.")
            .mandatory(true);

    ap.add_key_arg("-r", "--recursive")
            .description("Execute the process in a recursive way.");

    ap.add_key_value_arg("--seconds", "-s")
            .description("Set seconds.")
            .values_names("INTEGER")
            .mandatory(true);

    ap.add_key_value_arg("--minutes", "-m")
            .description("Set minutes.")
            .values_names("INTEGER")
            .mandatory(true)
            .minmax_values(2, 4);

    ap.add_key_value_arg("--hours", "-h")
            .description("Set hours.")
            .values_names("INTEGER")
            .minmax_values(0, 4);

    ap.add_keyless_arg("DESTINATION")
            .description("Destination directory.");

    ap.print_help();

    EXPECT_TRUE(ios_redirect.get_internal_string() == expected_res);
}


TEST_F(argparse_arg_parser, print_commands)
{
    std::string expected_res =
            "Commands:\n"
            "  -l, --long                Display the list in a not compacted mode.\n"
            "  -m, --minutes=INTEGER...  Set minutes Lorem ipsum dolor sit amet, consectetur\n"
            "                              adipiscing elit. Sed ullamcorper metus non nisi\n"
            "                              mattis, sit amet vestibulum ipsum vulputate.\n"
            "                              Quisque sollicitudin enim a felis vehicula, quis\n"
            "                              faucibus mi molestie. Fusce id justo et tortor\n"
            "                              vehicula porttitor.\n"
            "  -s, --seconds=INTEGER     Set seconds.\n\n";

    speed::iostream::ios_redirect ios_redirect(std::cout);
    ios_redirect.redirect_to_internal_stream();

    ap.configure()
            .program_name("speed");

    ap.add_help_menu()
            .print_usage(false)
            .print_options(false)
            .print_commands(true)
            .print_values(false);

    ap.add_key_arg("-a", "--all")
            .description("Display all the information.");

    ap.add_key_arg("-l", "--long")
            .description("Display the list in a not compacted mode.")
            .mandatory(true);

    ap.add_key_arg("-r", "--recursive")
            .description("Execute the process in a recursive way.");

    ap.add_key_value_arg("--minutes", "-m")
            .description("Set minutes Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed "
                         "ullamcorper metus non nisi mattis, sit amet vestibulum ipsum vulputate. "
                         "Quisque sollicitudin enim a felis vehicula, quis faucibus mi molestie. "
                         "Fusce id justo et tortor vehicula porttitor.")
            .values_names("INTEGER")
            .mandatory(true)
            .minmax_values(2, 4);

    ap.add_key_value_arg("--seconds", "-s")
            .description("Set seconds.")
            .values_names("INTEGER")
            .mandatory(true);

    ap.add_key_value_arg("--hours", "-h")
            .description("Set hours.")
            .values_names("INTEGER")
            .minmax_values(0, 4);

    ap.add_keyless_arg("DESTINATION")
            .description("Destination directory.");

    ap.print_help();

    EXPECT_TRUE(ios_redirect.get_internal_string() == expected_res);
}


TEST_F(argparse_arg_parser, print_values)
{
    std::string expected_res =
        "Values:\n"
        "  DESTINATION  Destination directory.\n\n";

    speed::iostream::ios_redirect ios_redirect(std::cout);
    ios_redirect.redirect_to_internal_stream();

    ap.configure()
            .program_name("speed");

    ap.add_help_menu()
            .print_usage(false)
            .print_options(false)
            .print_commands(false)
            .print_values(true);

    ap.add_key_arg("-a", "--all")
            .description("Display all the information.");

    ap.add_key_arg("-l", "--long")
            .description("Display the list in a not compacted mode.");

    ap.add_key_arg("-r", "--recursive")
            .description("Execute the process in a recursive way.");

    ap.add_key_value_arg("--seconds", "-s")
            .description("Set seconds.")
            .values_names("INTEGER")
            .mandatory(true);

    ap.add_key_value_arg("--minutes", "-m")
            .description("Set minutes.")
            .values_names("INTEGER")
            .mandatory(true)
            .minmax_values(2, 4);

    ap.add_key_value_arg("--hours", "-h")
            .description("Set hours.")
            .values_names("INTEGER")
            .mandatory(true)
            .minmax_values(0, 4);

    ap.add_keyless_arg("DESTINATION")
            .description("Destination directory.");

    ap.print_help();

    EXPECT_TRUE(ios_redirect.get_internal_string() == expected_res);
}


TEST_F(argparse_arg_parser, print_epilogue)
{
    std::string expected_res =
            "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed ullamcorper metus\n"
            "non nisi mattis, sit amet vestibulum ipsum vulputate. Quisque sollicitudin enim\n"
            "a felis vehicula, quis faucibus mi molestie. Fusce id justo et tortor vehicula\n"
            "porttitor.\n\n";

    speed::iostream::ios_redirect ios_redirect(std::cout);
    ios_redirect.redirect_to_internal_stream();

    ap.configure()
            .program_name("speed");

    ap.add_help_menu()
            .print_usage(false)
            .print_options(false)
            .print_commands(false)
            .print_values(false)
            .epilogue(
                    "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed ullamcorper metus"
                    " non nisi mattis, sit amet vestibulum ipsum vulputate. Quisque sollicitudin "
                    "enim a felis vehicula, quis faucibus mi molestie. Fusce id justo et tortor "
                    "vehicula porttitor.");

    ap.add_key_arg("-a", "--all")
            .description("Display all the information.");

    ap.print_help();

    EXPECT_TRUE(ios_redirect.get_internal_string() == expected_res);
}


TEST_F(argparse_arg_parser, print_help)
{
    std::string expected_res =
            "Usage: speed [OPTION]... -a --seconds=INTEGER --hours[=INTEGER]... DESTINATION\n"
            "\n"
            "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed ullamcorper metus\n"
            "non nisi mattis, sit amet vestibulum ipsum vulputate. Quisque sollicitudin enim\n"
            "a felis vehicula, quis faucibus mi molestie. Fusce id justo et tortor vehicula\n"
            "porttitor.\n"
            "\n"
            "Options:\n"
            "  -l, --long                Display the list in a not compacted mode.\n"
            "  -r, --recursive           Execute the process in a recursive way.\n"
            "  -m, --minutes=INTEGER...  Set minutes.\n"
            "\n"
            "Commands:\n"
            "  -a, --all                 Display all the information.\n"
            "  -s, --seconds=INTEGER     Set seconds.\n"
            "  -h, --hours[=INTEGER]...  Set hours.\n"
            "\n"
            "Values:\n"
            "  DESTINATION               Destination directory.\n"
            "\n"
            "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed ullamcorper metus\n"
            "non nisi mattis, sit amet vestibulum ipsum vulputate. Quisque sollicitudin enim\n"
            "a felis vehicula, quis faucibus mi molestie. Fusce id justo et tortor vehicula\n"
            "porttitor.\n\n";

    speed::iostream::ios_redirect ios_redirect(std::cout);
    ios_redirect.redirect_to_internal_stream();

    ap.configure()
            .program_name("speed");

    ap.add_help_menu()
            .print_usage(true)
            .print_options(true)
            .description(
                    "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed ullamcorper metus"
                    " non nisi mattis, sit amet vestibulum ipsum vulputate. Quisque sollicitudin "
                    "enim a felis vehicula, quis faucibus mi molestie. Fusce id justo et tortor "
                    "vehicula porttitor.")
            .print_commands(true)
            .print_values(true)
            .epilogue(
                    "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed ullamcorper metus"
                    " non nisi mattis, sit amet vestibulum ipsum vulputate. Quisque sollicitudin "
                    "enim a felis vehicula, quis faucibus mi molestie. Fusce id justo et tortor "
                    "vehicula porttitor.");

    ap.add_key_arg("-a", "--all")
            .description("Display all the information.")
            .mandatory(true);

    ap.add_key_arg("-l", "--long")
            .description("Display the list in a not compacted mode.");

    ap.add_key_arg("-r", "--recursive")
            .description("Execute the process in a recursive way.");

    ap.add_key_value_arg("--seconds", "-s")
            .description("Set seconds.")
            .values_names("INTEGER")
            .mandatory(true);

    ap.add_key_value_arg("--minutes", "-m")
            .description("Set minutes.")
            .values_names("INTEGER")
            .minmax_values(2, 4);

    ap.add_key_value_arg("--hours", "-h")
            .description("Set hours.")
            .values_names("INTEGER")
            .mandatory(true)
            .minmax_values(0, 4);

    ap.add_keyless_arg("DESTINATION")
            .description("Destination directory.");

    ap.print_help();

    EXPECT_TRUE(ios_redirect.get_internal_string() == expected_res);
}


TEST_F(argparse_arg_parser, print_version)
{
    std::string expected_res = "v1.0.0\n";

    speed::iostream::ios_redirect ios_redirect(std::cout);
    ios_redirect.redirect_to_internal_stream();

    ap.configure()
            .program_name("speed");

    ap.add_version_arg("-v", "--version")
            .description("Display version information.")
            .version_information("v1.0.0");

    ap.print_version();

    EXPECT_TRUE(ios_redirect.get_internal_string() == expected_res);
}


TEST_F(argparse_arg_parser, print_number_errors)
{
    std::string expected_res = "speed: --seconds: Invalid number '4896K'\n";
    std::uint32_t val;

    std::vector<const char*> argv = {
            "speed",
            "-s", "4896K"
    };

    speed::iostream::ios_redirect ios_redirect(std::cout);
    ios_redirect.redirect_to_internal_stream();

    ap.add_key_value_arg("--seconds", "-s")
            .description("Set seconds.")
            .values_names("INTEGER")
            .store_into(&val);

    ap.parse_args(argv.size(), argv);
    ap.print_errors();

    EXPECT_TRUE(ios_redirect.get_internal_string() == expected_res);
}


TEST_F(argparse_arg_parser, change_prefix)
{
    std::string expected_res =
            "Usage: speed [OPTION]... /a ##seconds=INTEGER ##hours[=INTEGER]... DESTINATION\n"
            "\n"
            "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed ullamcorper metus\n"
            "non nisi mattis, sit amet vestibulum ipsum vulputate. Quisque sollicitudin enim\n"
            "a felis vehicula, quis faucibus mi molestie. Fusce id justo et tortor vehicula\n"
            "porttitor.\n"
            "\n"
            "Options:\n"
            "  /l, ##long                Display the list in a not compacted mode.\n"
            "  /r, ##recursive           Execute the process in a recursive way.\n"
            "  /m, ##minutes=INTEGER...  Set minutes.\n"
            "\n"
            "Commands:\n"
            "  /a, ##all                 Display all the information.\n"
            "  /s, ##seconds=INTEGER     Set seconds.\n"
            "  /h, ##hours[=INTEGER]...  Set hours.\n"
            "\n"
            "Values:\n"
            "  DESTINATION               Destination directory.\n"
            "\n"
            "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed ullamcorper metus\n"
            "non nisi mattis, sit amet vestibulum ipsum vulputate. Quisque sollicitudin enim\n"
            "a felis vehicula, quis faucibus mi molestie. Fusce id justo et tortor vehicula\n"
            "porttitor.\n\n";

    speed::iostream::ios_redirect ios_redirect(std::cout);
    ios_redirect.redirect_to_internal_stream();

    ap.configure()
            .program_name("speed");

    ap.add_help_menu()
            .print_usage(true)
            .print_options(true)
            .description(
                    "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed ullamcorper metus"
                    " non nisi mattis, sit amet vestibulum ipsum vulputate. Quisque sollicitudin "
                    "enim a felis vehicula, quis faucibus mi molestie. Fusce id justo et tortor "
                    "vehicula porttitor.")
            .print_commands(true)
            .print_values(true)
            .epilogue(
                    "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed ullamcorper metus"
                    " non nisi mattis, sit amet vestibulum ipsum vulputate. Quisque sollicitudin "
                    "enim a felis vehicula, quis faucibus mi molestie. Fusce id justo et tortor "
                    "vehicula porttitor.");

    ap.add_key_arg("/a", "##all")
            .description("Display all the information.")
            .mandatory(true);

    ap.add_key_arg("/l", "##long")
            .description("Display the list in a not compacted mode.");

    ap.add_key_arg("/r", "##recursive")
            .description("Execute the process in a recursive way.");

    ap.add_key_value_arg("##seconds", "/s")
            .description("Set seconds.")
            .values_names("INTEGER")
            .mandatory(true);

    ap.add_key_value_arg("##minutes", "/m")
            .description("Set minutes.")
            .values_names("INTEGER")
            .minmax_values(2, 4);

    ap.add_key_value_arg("##hours", "/h")
            .description("Set hours.")
            .values_names("INTEGER")
            .mandatory(true)
            .minmax_values(0, 4);

    ap.add_keyless_arg("DESTINATION")
            .description("Destination directory.");

    ap.configure()
            .short_prefixes("/")
            .long_prefixes("##");

    ap.print_help();

    EXPECT_TRUE(ios_redirect.get_internal_string() == expected_res);
}
