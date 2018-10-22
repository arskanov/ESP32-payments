### Application file for server

import sys
import falcon

from server.resource.request import Request

from server.storage import db


api = application = falcon.API()

if db.ok() == False:
    sys.exit()

def routes():
    base_url="/api/v1"
    r = Request()
    api.add_route(base_url+"/test",r)

routes()
