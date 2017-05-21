/*
 * Copyright (c) 2017 Wes Hampson <thehambone93@gmail.com>
 *
 * Licensed under the MIT License. See LICENSE at top level directory.
 */

#ifndef _GXTMAKER_GXTMAKER_H_
#define _GXTMAKER_GXTMAKER_H_

#define GXTMAKER_VERSION_MAJOR 0
#define GXTMAKER_VERSION_MINOR 2
#define GXTMAKER_VERSION_PATCH 0
#define GXTMAKER_VERSION_BUILD ""  /* Additional build info goes here. */

#define GXTMAKER_APP_NAME  "gxtmaker"
#define GXTMAKER_APP_MOTTO "GTA Text Compiler"

#define GXTMAKER_HELP_MESSAGE \
"Usage: " GXTMAKER_APP_NAME " [options] file\n\
\nOptions:\n\
    --help      show this help menu and exit\n\
    --version   display program version information and exit"

#define GXTMAKER_COPYRIGHT_NOTICE \
"Copyright (c) 2017 Wes Hampson <thehambone93@gmail.com>"

#define GXTMAKER_LICENSE_NOTICE \
"This software is licensed under the MIT License."

#define GXTMAKER_WARRANTY_NOTICE \
"THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\n\
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\n\
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE\n\
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\n\
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\n\
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE\n\
SOFTWARE."

enum exit_status
{
    GXTMAKER_EXIT_SUCCESS           = 0,
    GXTMAKER_EXIT_ARGUMENT_ERROR    = 1,
    GXTMAKER_EXIT_IO_ERROR          = 2,
    GXTMAKER_EXIT_COMPILATION_ERROR = 3
};

#endif /* _GXTMAKER_GXTMAKER_H_ */
