/* ===========================================================================
 *
 *                            PUBLIC DOMAIN NOTICE
 *               National Center for Biotechnology Information
 *
 *  This software/database is a "United States Government Work" under the
 *  terms of the United States Copyright Act.  It was written as part of
 *  the author's official duties as a United States Government employee and
 *  thus cannot be copyrighted.  This software/database is freely available
 *  to the public for use. The National Library of Medicine and the U.S.
 *  Government have not placed any restriction on its use or reproduction.
 *
 *  Although all reasonable efforts have been taken to ensure the accuracy
 *  and reliability of the software and data, the NLM and the U.S.
 *  Government do not and cannot warrant the performance or results that
 *  may be obtained by using this software or data. The NLM and the U.S.
 *  Government disclaim all warranties, express or implied, including
 *  warranties of performance, merchantability or fitness for any particular
 *  purpose.
 *
 *  Please cite the author in any work or product based on this material.
 *
 * ===========================================================================
 *
 * Project:
 *  sratools command line tool
 *
 * Purpose:
 *  argv manipulations
 *
 */

#pragma once
#include <tuple>
#include <string>
#include <vector>
#include <map>
#include <iterator>
#include <algorithm>

#include "opt_string.hpp"

namespace sratools {

using Dictionary = std::map<std::string, std::string>;
using ParamList = std::vector<std::pair<std::string, opt_string>>;
using ArgsList = std::vector<std::string>;

static inline
ParamList::const_iterator getParamValue(ParamList::value_type::first_type const &param, ParamList const &params)
{
    return std::find_if(params.begin(), params.end(), [&](ParamList::value_type const &value) {
        return value.first == param;
    });
}

static inline
bool hasParamValue(ParamList::value_type::first_type const &param, ParamList const &params)
{
    return std::find_if(params.begin(), params.end(), [&](ParamList::value_type const &value) {
        return value.first == param;
    }) != params.end();
}

/// @brief checks if the current item matches and extracts the value if it does
///
/// @param param the wanted parameter, e.g. '--option-file'
/// @param current the current item
/// @param end the end, e.g. args.end()
///
/// @returns if found, tuple<true, this is the value, the next item to resume at>. if not found, tuple<false, ???, ???>
extern
std::tuple<bool, std::string, ArgsList::iterator> matched(std::string const &param, std::string const &value_type, ArgsList::iterator current, ArgsList::iterator end);


/// @brief load argv, handle any option files
///
/// @param argc argc - 1
/// @param argv argv + 1
///
/// @returns the args. May throw on I/O error or parse error.
extern
ArgsList loadArgv(int argc, char *argv[]);

using ArgsInfo = std::pair<Dictionary, Dictionary>;

enum ParseArgsResult {
    ok,     ///< success
    failed, ///< unrecognized parameter or missing argument
    help,   ///< user asked for help
    version ///< user asked for version
};

/// @brief splits argv into parameters and arguments
///
/// @param parameters [out] recieves the parameters, always translated to long form
/// @param arguments [out] recieves the arguments
/// @param info information need to parse, e.g. what parameters take arguments
///
/// @return false if not parsed okay or help, else true
extern
ParseArgsResult parseArgs(  ParamList *parameters
               , ArgsList *arguments
               , ArgsInfo const &info
               );

} // namespace sratools
