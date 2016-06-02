
# Lora server
# Currently just logs and displays all the stuff it gets

import os
from os.path import join as pjoin
base = os.path.dirname(__file__)

from datetime import datetime

import tornado
import tornado.ioloop
import tornado.web

ioloop = tornado.ioloop.IOLoop.current()

tornado_settings = {
    "template_path": pjoin(base, "templates"),
    "autoreload": True
}

posts_dir = pjoin(base, "logs")

class PostLog:
    def __init__(self, name, content):
        self.name = name
        self.content = content

class MainHandler(tornado.web.RequestHandler):
    
    def get(self):
        filenames = sorted(filter(lambda f: os.path.isfile(pjoin(posts_dir, f)), os.listdir(posts_dir)), reverse=True)
        logs = []
        for fname in filenames:
            with open(pjoin(posts_dir, fname), "r") as f:
                logs.append(PostLog(fname, f.read()))
        
        self.render("index.html", logs=logs)
        
    
    def post(self):
        fname = pjoin(posts_dir, str(datetime.now()) + ".txt")
        with open(fname, "w") as f:
            f.write(self.request.body.decode("utf8"))
        
        self.write("Thanks and have a nice day.")

def make_app():
    return tornado.web.Application([
        (r"/", MainHandler),
    ], **tornado_settings)


if __name__ == "__main__":
    app = make_app()
    app.listen(80)
    ioloop.start()
