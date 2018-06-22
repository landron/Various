'''
    Problem:    "This application will read an iTunes export file in XML
                 and produce a properly normalized database with this structure:"
    model:  http://www.pythonlearn.com/code/tracks.zip
        Most of this file is not my code.

    upgraded to python 3

    >pylint --version
    No config file found, using default configuration
    pylint 1.5.5,
    astroid 1.4.5
    Python 3.5.1 (v3.5.1:37a07cee5969, Dec  6 2015, 01:38:48) [MSC v.1900 32 bit (Intel)]

    "Your code has been rated at 10.00/10"
'''

import xml.etree.ElementTree as ET
import sqlite3

def main():
    '''main function, the program'''
    conn = sqlite3.connect('trackdb.sqlite')
    cur = conn.cursor()

    # Make some fresh tables using executescript()
    cur.executescript('''
    DROP TABLE IF EXISTS Artist;
    DROP TABLE IF EXISTS Genre;
    DROP TABLE IF EXISTS Album;
    DROP TABLE IF EXISTS Track;

    CREATE TABLE Artist (
        id  INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
        name    TEXT UNIQUE
    );

    CREATE TABLE Genre (
        id  INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
        name    TEXT UNIQUE
    );

    CREATE TABLE Album (
        id  INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
        artist_id  INTEGER,
        title   TEXT UNIQUE
    );

    CREATE TABLE Track (
        id  INTEGER NOT NULL PRIMARY KEY 
            AUTOINCREMENT UNIQUE,
        title TEXT  UNIQUE,
        album_id  INTEGER,
        genre_id  INTEGER,
        len INTEGER, rating INTEGER, count INTEGER
    );
    ''')

    fname = input('Enter file name: ')
    if len(fname) < 1:
        fname = 'Library.xml'

    # <key>Track ID</key><integer>369</integer>
    # <key>Name</key><string>Another One Bites The Dust</string>
    # <key>Artist</key><string>Queen</string>
    def lookup(dic_track, key):
        '''find the value for the key in XML'''
        found = False
        for child in dic_track:
            if found:
                return child.text
            if child.tag == 'key' and child.text == key:
                found = True
        return None

    def add_next_track(entry, cur):
        '''transfer the next track (XML entry) to the database (cursor)'''

        name = lookup(entry, 'Name')
        artist = lookup(entry, 'Artist')
        album = lookup(entry, 'Album')
        count = lookup(entry, 'Play Count')
        rating = lookup(entry, 'Rating')
        length = lookup(entry, 'Total Time')
        genre = lookup(entry, 'Genre')

        if name is None or artist is None or album is None or not genre:
            return False

        print(name, artist, album, genre, count, rating, length)

        cur.execute('''INSERT OR IGNORE INTO Artist (name) \
            VALUES ( ? )''', (artist, ))
        cur.execute('SELECT id FROM Artist WHERE name = ? ', (artist, ))
        artist_id = cur.fetchone()[0]

        cur.execute('''INSERT OR IGNORE INTO Album (title, artist_id) \
            VALUES ( ?, ? )''', (album, artist_id))
        cur.execute('SELECT id FROM Album WHERE title = ? ', (album, ))
        album_id = cur.fetchone()[0]

        cur.execute('''INSERT OR IGNORE INTO Genre (name) \
            VALUES ( ? )''', (genre, ))
        cur.execute('SELECT id FROM Genre WHERE name = ? ', (genre, ))
        genre_id = cur.fetchone()[0]

        cur.execute('''INSERT OR REPLACE INTO Track \
            (title, album_id, genre_id, len, rating, count) \
            VALUES ( ?, ?, ?, ?, ?, ? )''', \
            (name, album_id, genre_id, length, rating, count))

        return True

    stuff = ET.parse(fname)
    all_tracks = stuff.findall('dict/dict/dict')
    print('Dict count:', len(all_tracks))
    for entry in all_tracks:
        if lookup(entry, 'Track ID') is None:
            continue
        add_next_track(entry, cur)

    conn.commit()

    cur.close()

if __name__ == "__main__":
    main()
