/* Webcamoid, webcam capture application.
 * Copyright (C) 2011-2017  Gonzalo Exequiel Pedone
 *
 * Webcamoid is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Webcamoid is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Webcamoid. If not, see <http://www.gnu.org/licenses/>.
 *
 * Web-Site: http://webcamoid.github.io/
 */

#include <CoreFoundation/CFRunLoop.h>

#include "assistant.h"
#include "VCamUtils/src/utils.h"

AkVCam::Assistant *assistant()
{
    static AkVCam::Assistant assistant;

    return &assistant;
}

int main()
{
    auto server =
            xpc_connection_create_mach_service(AKVCAM_ASSISTANT_NAME,
                                               NULL,
                                               XPC_CONNECTION_MACH_SERVICE_LISTENER);

    if (!server)
        return EXIT_FAILURE;

    xpc_connection_set_event_handler(server, ^(xpc_object_t event) {
        auto type = xpc_get_type(event);

        if (type == XPC_TYPE_ERROR) {
             auto description = xpc_copy_description(event);
             AkLoggerLog("ERROR: " << description);
             free(description);

             return;
        }

        auto client = reinterpret_cast<xpc_connection_t>(event);

        xpc_connection_set_event_handler(client, ^(xpc_object_t event) {
            assistant()->messageReceived(client, event);
        });

        xpc_connection_resume(client);
    });

    xpc_connection_resume(server);
    CFRunLoopRun();
    xpc_release(server);

    return EXIT_SUCCESS;
}
