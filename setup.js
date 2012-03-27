/*
 *	setup for parannus
 *	creates admin user and imports pkgs
 */

var util = require('./lib/util.js'),
	config = require('./config.js'),
	db = require('./lib/db.js'),
	setup = require("./lib/setup.js");



console.log(util.system_id);

var an = require("./lib/an.js");

/*db.store_ht("lamekey","mycontnet",function(err,data){
  console.log("calling get for:",data);
  db.get_ht("lamekey", function(err,data) {
    console.log("got:",data);
  });
});
*
db.store_with_id("lamekey",{a:"mycontnet"},function(err,data){
  console.log("calling get for:",data.id);
  db.get(data.id, function(err,data) {
    console.log("got:",data);
  });
});
*/


console.log("scan packages...");
setup.ScanLocalPackages(function(err,mlid){
	console.log("packages scaned");
		console.log("storing master list...",mlid);
		/*db.store_ht(util.system_id+"masterlist",mlid, function(err){
			if(err)
				throw new Error("could not save masterlist");
*/
			setup.createDefaultRepo(mlid,function(err,res){
				if(err)
					throw err;
				console.log("repo created!");
				db.close();
			});
		//});
});

