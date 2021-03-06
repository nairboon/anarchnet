/*
 * Copyright (C) 2010, 2011 Remo Hertig (nairboon)
 * https://launchpad.net/anarchNet
 *
 * This file is part of anarchNet.
 *
 * anarchNet is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * anarchNet is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with anarchNet.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "db.h"
#include "singleton.h"

#ifndef SRC_DAEMON_DB_MANAGER_H_
#define SRC_DAEMON_DB_MANAGER_H_
namespace an {

class DBManager : public Singleton<DBManager> {
		friend class Singleton<DBManager>;
public:
		bool init(const std::string&);

		bool create_object(db::ObjPtr obj);
		bool update_object(db::ObjPtr obj,const String& diff);
		bool update_object(const db::ObjID& id,const String& diff);
		bool delete_object(const db::ObjID& id);
		bool get_object_head(const db::ObjID& id,db::ObjPtr res);
		bool rollback_object(const db::ObjPtr obj,const db::ObjID& rev);
		bool get_object_rev(const db::ObjPtr obj,const db::ObjID& rev,String& res);

		std::vector<std::string> get_unchecked_keys_since(int);
private:

	};
}
#endif  // SRC_DAEMON_DB_MANAGER_H_
