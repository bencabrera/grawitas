"use strict";

var express = require("express");
var http = require("http");
var path = require("path");
var exec = require('child_process').exec;
var fs = require('fs');
var app = express();
var server = http.createServer(app);

// use body parser for parsin POST data
var bodyParser = require('body-parser');
app.use(bodyParser.json({ limit: '50mb' })); // support json encoded bodies
app.use(bodyParser.urlencoded({ extended: true, limit: '50mb' })); // support encoded bodies

var router = express.Router();

router.get('/', function(req, res, next) {
    res.sendFile("index.html", { root: './' });
});

router.post('/parse', function(req, res, next) {
	var command = path.join(__dirname, "../bin/debug", "WikiTalkPageParser");
	var parameters = " --input-talk-page " + path.join(__dirname, "./local.wikisyntax") + " --" + req.body.format + " " + path.join(__dirname, "./out.txt");

	fs.writeFile(path.join(__dirname,"./local.wikisyntax"), req.body.syntax, function() {
		exec(command + parameters, function(error, stdout, stderr) {
			if(error instanceof Error) {
				console.error("ERROR: ", error);
				return res.status(503).json({ error: "Not implemented" });
			} else {
				return res.sendFile("./out.txt", { root: './' });
			}
		});
	});
});
app.use(router);
app.use(express.static('./'));
// function routerFallback(req, res) {
// res.sendFile("index.html", { root: './angular/' });
// }
// app.use(routerFallback);

// catch 404 and forward to error handler
app.use(function(req, res, next) {
	var err = new Error('Not Found');
	err['status'] = 404;
	next(err);
	next();
});
server.listen(3000, function () {
	var host = "localhost";
	var port = server.address().port;
	console.log('App listening at http://%s:%s', host, port);
});
module.exports = app;
//# sourceMappingURL=server.js.map
