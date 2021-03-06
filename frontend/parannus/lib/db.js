var hashlib = require('hashlib2'),
	config = require('../config.js'),
	dmp = require('./diff_match_patch.js'),
	an = require('./rpc.js'),
	net = require("net");



var LocalDatabase = function() {
		
};

LocalDatabase.prototype = {
	registerUser: function(param,cb) {
		
	},
	authorizeUser: function(param,cb) {
		
	},
	getX: function(id,branch,rev,completeDocument,cb) {
		throw "NENENEN";
		Data.findOne({'id': id},function(err,res) {
			if(err || !res) {
				return cb(err,res);
			}
			if(completeDocument)
				return	cb(null,res);

				if(branch)
					branch_id = branch;
				else
					branch_id = res.head;

			res.branch = res.branches.id(branch_id);
			res.snapshot = res.branch.snapshots.id(res.branch.head);

			var diffs = [];
			var snap = res.snapshot.content;
			var newc = snap;
			var patch;
			for (var i = 0, l = res.branch.diffs.length; i < l; i++) {
			    if (ObjectId.toString(res.branch.diffs[i].snapshot) == ObjectId.toString(res.snapshot._id)) {
			      patch = dmp.patch_fromText(res.branch.diffs[i].content);
					newc = dmp.patch_apply(patch,newc)[0];
				}
			  }

			res.content = newc;
			return cb(null,res);
		});
	},
	updateX: function(id,newcontent,branch,session,cb) {
	throw "NENENEN";
		if(!branch)
			throw new Error("Branch cannot be null!");
		//FIXME: check ownership
		this.get(id,branch,null,false,function(err,res) {
				if(err || !res)
					return cb(err,null);

			var patch = dmp.patch_make(res.content,newcontent);
			var diff = dmp.patch_toText(patch);
			console.log(diff);

			dobj = new Diff();
			dobj.snapshot = res.snapshot._id;
			dobj.content = diff;
			res.content = newcontent;
			res.branches.id(branch).diffs.push(dobj);
			res.save(function(err){
				if(err)
					cb(err,null);
				else
					cb(null,res);
			});
		});
	},
	storeOLD: function(param,cb) {


		var x = {data: param.content, owner: param.owner};
		an.call("object.create",x,function(err,res) {
		  console.log(err,res);
		  if(err)
		    throw err;

		  cb(err,res);
		});
		return;

		var data = new Snapshot();
		data.content = param.content;
		data.save();

		var branch = new Branch();
		branch.snapshots.push(data);
		branch.head = data._id;
		branch.save();

		var ins = new Data();
		//ins.type = param.type;
		ins.id = hashlib.sha256(new Date()+param.owner+Math.random()); // TODO: + username, +usercount+ servername
		ins.owner = param.owner;
		ins.branches.push(branch);
		ins.head = branch._id;
		ins.save(function(err) {
			if(err)
				cb(err,null);
			else
				cb(null,ins);
		});
		console.log("new data: "+ins._id);
	},
	deleteX: function(id,userid,cb) {
	throw "NENENEN";
			Data.findOne({'id': id, owner: userid},function(err,res) {
				if(err || !res)
					return cb(err,null);

				res.remove(function(err) {
					if(err)
						cb(err,null);
					else
						cb(null,res.id);
				});
			});
	},
	get_nohash: function(id,cb) {
		this.get(hashlib.sha256(id),cb);
	},
	get: function(k,c) { get(k,c,false); },
	get: function(key,cb,full) {
	an.call("object.get",{id: key,full:full},function(err,data) {
	    if(err) {
	      console.log(err);
	      cb(err,data);
	    }
	    else {
	      //console.log(data);
	      console.log(new Buffer(data.content,'base64').toString('utf-8'));
	      cb(err,JSON.parse(new Buffer(data.content,'base64').toString('utf-8')));
	      }
	    });
	},
	get_ht: function(key,cb) {
	an.call("kv.get",{key: key},function(err,data) {
	    if(err) {
	      console.log(err);
	      cb(err,data);
	    }
	    else {
	      cb(err,new Buffer(data.content,'base64').toString('utf-8'));
	      }
	    });
	},
	store: function(content,cb) {
		if(!content instanceof String)
			throw new Error("store.content != string");
		var id = hashlib.sha256(JSON.stringify(content));
		console.log("store:",id);
		an.call("object.create",{id:id,content:new Buffer(JSON.stringify(content)).toString('base64')},cb);
	},
	store_with_id: function(key,content,cb) {
		if(!content instanceof Object)
			throw new Error("store_with_id.content != object");
	an.call("object.create",{id: key,content:new Buffer(JSON.stringify(content)).toString('base64')},cb);
	},
	store_ht: function(key,content,cb) {
	console.log("store_ht:",key);
	an.call("kv.put",{key: key, value:new Buffer(content).toString('base64')},function(err,data) {
	    if(err) {
	      console.log(err);
	    }
	      console.log(data);
	      cb(err,data);
	    });
	},
	get_file: function(key,cb) {
	console.log("get_file:",key);
	an.call("file.get",{id: key},function(err,data) {
	    if(err) {
	      console.log(err);
	    }
	      console.log(data);
	      cb(err,data);
	    });
	},
	
	put_file: function(path,cb) {
	console.log("put_file:",path);
	an.call("file.put",{path: path},function(err,data) {
	    if(err) {
	      console.log(err);
	    }
	      console.log(data);
	      cb(err,data);
	    });
	},
	
	
	close: function() { an.close(); }
};

var ldb = new LocalDatabase();

var dbObj = function (id,content) {
	this.id = id;
	this.obj = content;
}

var odb = function () {
}
odb.prototype = {
	get: function(id,cb) {
		ldb.get(id,function(err,res) {
			if(err)
				throw err + " for " + id;
			cb(new dbObj(id,res));
		});
	},
	create: function(content) {
		return new dbObj("", content);
	}
};

dbObj.prototype = {

	save: function(cb) {
		if(this.id==="") { //new object
			ldb.store.call(this,this.obj,function(err,res) {
				if(err)
					throw err;
				id = res.id;
				console.log(res);
				cb.call(this,err,this);
			});
		}
		ldb.store_with_id(this.id,this.obj,cb);
	},
	load: function(id,cb) {
		this.id = id;
		ldb.get(id,function(err,res) {
			if(err)
				throw err + " for " + id;
			this.obj = res;
			cb(err,res);
		});
	}
};

module.exports = ldb;
module.exports.obj = dbObj;
module.exports.odb = new odb();
