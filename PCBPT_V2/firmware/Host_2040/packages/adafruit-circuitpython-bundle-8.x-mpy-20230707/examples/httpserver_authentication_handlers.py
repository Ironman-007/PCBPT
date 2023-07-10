# SPDX-FileCopyrightText: 2022 Dan Halbert for Adafruit Industries
#
# SPDX-License-Identifier: Unlicense

import socketpool
import wifi

from adafruit_httpserver import Server, Request, Response, UNATUHORIZED_401
from adafruit_httpserver.authentication import (
    AuthenticationError,
    Basic,
    Bearer,
    check_authentication,
    require_authentication,
)


pool = socketpool.SocketPool(wifi.radio)
server = Server(pool, debug=True)

# Create a list of available authentication methods.
auths = [
    Basic("user", "password"),
    Bearer("642ec696-2a79-4d60-be3a-7c9a3164d766"),
]


@server.route("/check")
def check_if_authenticated(request: Request):
    """
    Check if the request is authenticated and return a appropriate response.
    """
    is_authenticated = check_authentication(request, auths)

    return Response(
        request,
        body="Authenticated" if is_authenticated else "Not authenticated",
        content_type="text/plain",
    )


@server.route("/require-or-401")
def require_authentication_or_401(request: Request):
    """
    Require authentication and return a default server 401 response if not authenticated.
    """
    require_authentication(request, auths)

    return Response(request, body="Authenticated", content_type="text/plain")


@server.route("/require-or-handle")
def require_authentication_or_manually_handle(request: Request):
    """
    Require authentication and manually handle request if not authenticated.
    """

    try:
        require_authentication(request, auths)

        return Response(request, body="Authenticated", content_type="text/plain")

    except AuthenticationError:
        return Response(
            request,
            body="Not authenticated - Manually handled",
            content_type="text/plain",
            status=UNATUHORIZED_401,
        )


server.serve_forever(str(wifi.radio.ipv4_address))
