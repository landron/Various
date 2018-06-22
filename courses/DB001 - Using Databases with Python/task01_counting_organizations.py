'''
    Problem:    "This application will read the mailbox data (mbox.txt) count up the number \
                email messages per organization (i.e. domain name of the email address) \
                using a database"
    model:  http://www.pythonlearn.com/code/emaildb.py
        Most of this is not my code.

    upgraded to python 3

    >pylint --version
    No config file found, using default configuration
    pylint 1.5.5,
    astroid 1.4.5
    Python 3.5.1 (v3.5.1:37a07cee5969, Dec  6 2015, 01:38:48) [MSC v.1900 32 bit (Intel)]

    "Your code has been rated at 10.00/10"
'''

import sqlite3

def main():
    '''main function: solve the given task'''
    conn = sqlite3.connect('emaildb.sqlite')
    cur = conn.cursor()

    cur.execute('''
    DROP TABLE IF EXISTS Counts''')

    cur.execute('''
    CREATE TABLE Counts (org TEXT, count INTEGER)''')

    fname = input('Enter file name: ')
    if len(fname) < 1:
        fname = 'mbox.txt'
    fhandle = open(fname)
    for line in fhandle:
        if not line.startswith('From: '):
            continue
        pieces = line.split()
        email = pieces[1]
        pieces = email.split('@')
        org = pieces[1]
        print(org)
        cur.execute('SELECT count FROM Counts WHERE org = ? ', (org, ))
        row = cur.fetchone()
        if row is None:
            cur.execute('''INSERT INTO Counts (org, count) VALUES ( ?, 1 )''', (org, ))
        else:
            cur.execute('UPDATE Counts SET count=count+1 WHERE org = ?', (org, ))
    # This statement commits outstanding changes to disk each
    # time through the loop - the program can be made faster
    # by moving the commit so it runs only after the loop completes
    conn.commit()

    # https://www.sqlite.org/lang_select.html
    sqlstr = 'SELECT org, count FROM Counts ORDER BY count DESC LIMIT 10'

    print()
    print("Counts:")
    for row in cur.execute(sqlstr):
        print(str(row[0]), row[1])

    cur.close()

if __name__ == "__main__":
    main()
