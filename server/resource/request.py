
from falcon.media.validators.jsonschema import validate
import json

from server.schema import load_schema


from server.storage import db
from server.storage.models import Request as model_req


class Request:

    def on_get(self, req, resp):

        id = req.get_param("id")

        r = model_req()
        db.add(r)
        db.commit()


        status = {"status": {"1":"ok" }}
        resp.body=json.dumps(status)