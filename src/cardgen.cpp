/**
 * @file    cardgen.cpp
 * @author  Phil Lockett <phillockett65@gmail.com>
 * @version 1.0
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * https://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * 'cardgen' is a playing card image generator.
 *
 * System entry point for the card generator.
 */

#include "cardgen.h"
#include <sys/stat.h>
#include "Configuration.h"


/**
 * @section Global variables.
 *
 */

/**
 * System entry point.
 *
 * @param  argc - command line argument count.
 * @param  argv - command line argument vector.
 * @return error value or 0 if no errors.
 */
int main(int argc, char *argv[])
{
//- Get the command line parameters.
    int ret{Config::instance().init(argc, argv)};
    if (ret < 0)
        return 1;
    else if (ret > 0)
        return 0;

    if (!Config::isValid(true))
        return 1;

//- If all is well, generate the script.
    if (!ret)
    {
        generateScript(argc, argv);

        // Ensure output scripts are executable.
        chmod(Config::getScriptFilename().c_str(), S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH);
    }

    return 0;
}

