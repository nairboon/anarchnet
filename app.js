
/**
 * Module dependencies.
 */

var express = require('express');
var mongoose = require('mongoose');
var MongoStore = require('express-session-mongo');
require('express-resource');

//var DataProvider = require('./db.js').DataProvider;
//var dp = new DataProvider();
	
var app = module.exports = express.createServer();

// Configuration

app.configure(function(){
  app.set('views', __dirname + '/views');
  app.set('view engine', 'jade');
  app.use(express.bodyParser());
  app.use(express.methodOverride());
  app.use(express.cookieParser());
 // app.use(express.session({ secret: 'your secret here', store: new MongoStore({db: 'test'}) }));
  app.use(express.compiler({ src: __dirname + '/public', enable: ['less'] }));
  app.use(app.router);
  app.use(express.static(__dirname + '/public'));
});

app.configure('development', function(){
  app.use(express.errorHandler({ dumpExceptions: true, showStack: true })); 
  app.set('db-url', 'mongodb://localhost/test');
});

app.configure('production', function(){
  app.use(express.errorHandler()); 
});

var db = mongoose.connect(app.set('db-url'));
//console.log(db);


// Routes

app.get('/', function(req, res){
console.log("index");
/*  res.render('index', {
    title: 'Express'
  });*/
  console.log("ll");
});

app.get('/app/:id', function(req, res){
  res.render('index', {
    title: 'Express'
  });
});

app.get('/data/:id', function(req, res){
 // res.send(dp.getData(req.params.id));
});



//app.resource("edit",require("./editor"));

// Only listen on $ node app.js

if (!module.parent) {
  app.listen(3000);
  console.log("Express server listening on port %d", app.address().port);
}
