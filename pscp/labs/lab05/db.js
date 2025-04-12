class user {
    constructor(id, name, bday) {
        this.id = id;
        this.name = name;
        this.bday = bday;
    }
}

class DB {
    static async select(array) {
        return array.sort((a, b) => a.id - b.id);
    }

    static async insert(row) {
        let lastId = Math.max(...database.map(item => item.id), 0) + 1;
        const id = lastId;
        const newRow = { id, ...row };
        database.push(newRow);
        return newRow;
    }

    static async update(newData) {
        const index = database.findIndex(item => item.id === Number.parseInt(newData.id));
        if (index !== -1) {
            database.splice(index, 1);
            database.push(newData);
            return database[database.length - 1];
        }
        return null;
    }

    static async delete(idD) {
        const index = database.findIndex(item => item.id === Number.parseInt(idD));
        if (index !== -1) {
            return database.splice(index, 1)[0];
        }
        return null;
    }
    static async commit(){
        //console.log("commit")
    }
}

let database = [
    new user(1, "Vika", "2005-05-28"),
    new user(2, "Vivsi", "1982-02-11"),
    new user(3, "Vadim", "2005-04-22"),
    new user(4, "Katya", "2006-09-29"),
    new user(5, "Pavel", "1980-10-30")
];

module.exports = { user, DB, database };
