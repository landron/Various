'''
    Problem:    "This application will read roster data in JSON format, parse the file, \
                and then produce an SQLite database that contains a User, Course, and Member \
                table and populate the tables from the data file."
    model:  http://www.pythonlearn.com/code/roster.py
        Most of this file is not my code.

    upgraded to python 3

    >pylint --version
    No config file found, using default configuration
    pylint 1.5.5,
    astroid 1.4.5
    Python 3.5.1 (v3.5.1:37a07cee5969, Dec  6 2015, 01:38:48) [MSC v.1900 32 bit (Intel)]

    "Your code has been rated at 10.00/10"
'''

import json
import sqlite3

def main():
    '''main function, the program'''
    conn = sqlite3.connect('rosterdb.sqlite')
    cur = conn.cursor()

    # Do some setup
    cur.executescript('''
    DROP TABLE IF EXISTS User;
    DROP TABLE IF EXISTS Member;
    DROP TABLE IF EXISTS Course;

    CREATE TABLE User (
        id     INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
        name   TEXT UNIQUE
    );

    CREATE TABLE Course (
        id     INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
        title  TEXT UNIQUE
    );

    CREATE TABLE Member (
        user_id     INTEGER,
        course_id   INTEGER,
        role        INTEGER,
        PRIMARY KEY (user_id, course_id)
    )
    ''')
    conn.commit()

    fname = input('Enter file name: ')
    if len(fname) < 1:
        fname = 'roster_data.json'

    # [
    #   [ "Charley", "si110", 1 ],
    #   [ "Mea", "si110", 0 ],

    str_data = open(fname).read()
    json_data = json.loads(str_data)

    for entry in json_data:

        name = entry[0]
        title = entry[1]
        role = entry[2]

        print(name, title)

        cur.execute('''INSERT OR IGNORE INTO User (name) \
            VALUES ( ? )''', (name,))
        cur.execute('SELECT id FROM User WHERE name = ? ', (name, ))
        user_id = cur.fetchone()[0]

        cur.execute('''INSERT OR IGNORE INTO Course (title) \
            VALUES ( ? )''', (title,))
        cur.execute('SELECT id FROM Course WHERE title = ? ', (title, ))
        course_id = cur.fetchone()[0]

        cur.execute('''INSERT OR REPLACE INTO Member \
            (user_id, course_id, role) VALUES ( ?, ?, ? )''', \
            (user_id, course_id, role))

    conn.commit()

if __name__ == "__main__":
    main()
