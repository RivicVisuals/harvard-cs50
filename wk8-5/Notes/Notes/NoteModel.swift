//
//  NoteModel.swift
//  Notes
//
//  Created by Luka Rivic on 30.10.20.
//

import Foundation
import SQLite3

struct Note {
    var id: Int
    var noteTitle: String
    var content: String
    var date: String
}

class NoteManager {
    // keep track of the database
    var database: OpaquePointer!
    
    // make this a singleton
    static let main = NoteManager()
    private init() {
        connect()
    }
    
    // establishes a connection to the database.
    func connect() {
        // connection is established, don't do anything
        if database != nil {
            return
        }
        
        do {
            // create or read file notes.sqlite3
            let databaseURL = try FileManager.default.url(for: .documentDirectory, in: .userDomainMask, appropriateFor: nil, create: true).appendingPathComponent("notes.sqlite3")
            
            // open notes.sqlite3
            if sqlite3_open(databaseURL.path, &database) == SQLITE_OK {
                // check for the table notes
                if sqlite3_exec(database, "CREATE TABLE IF NOT EXISTS notes (title TEXT, contents TEXT, date INT)", nil, nil, nil) == SQLITE_OK {
                    
                } else {
                    print("Could not create table")
                }
            } else {
                print("Could not open database")
            }
        } catch {
            print("Could not create database")
        }
    }
    
    // inserts a new blank row.
    // called when user clicks on "Add" button
    func insertNote() -> Int {
        connect()
        
        var statement: OpaquePointer!
        
        // TODO: write date of change
        if sqlite3_prepare_v2(database, "INSERT INTO notes (title, contents, date) VALUES ('New note', '', datetime('now'))", -1, &statement, nil) != SQLITE_OK {
            print("Could not create note")
            return -1
        }
        if sqlite3_step(statement) != SQLITE_DONE {
            print("Could not create query")
            return -1
        }
        
        sqlite3_finalize(statement)

        return Int(sqlite3_last_insert_rowid(database))
    }
    
    // updates the passed note.
    // called when user navigates away from EditNoteView.
    func updateNote(note: Note) -> Int {
        connect()
        
        var statement: OpaquePointer!
        
        if sqlite3_prepare_v2(database, "UPDATE notes SET title = ?, contents = ?, date = (SELECT datetime('now')) WHERE rowid IS ?", -1, &statement, nil) != SQLITE_OK {
            print("Error creating update statement")
            return -1
        }
        
        // Bind title
        sqlite3_bind_text(statement, 1, NSString(string: note.noteTitle).utf8String, -1, nil)
        // Bind content
        sqlite3_bind_text(statement, 2, NSString(string: note.content).utf8String, -1, nil)
        // Bind rowid
        sqlite3_bind_int(statement, 3, Int32(note.id))
        
        if sqlite3_step(statement) != SQLITE_DONE {
            print("Error while saving note")
            return -1
        }
        
        sqlite3_finalize(statement)
        return 0
    }
    
    // deletes the passed note.
    // called when user deletes a note from the EditView or from the TableView list.
    func deleteNote(note: Note) -> Int {
        connect()
        
        var statement: OpaquePointer!
        
        if sqlite3_prepare_v2(database, "DELETE FROM notes WHERE rowid = ?", -1, &statement, nil) != SQLITE_OK {
            print("Error preparing delete statement")
            return -1
        }
        
        sqlite3_bind_int(statement, 1, Int32(note.id))
        
        if sqlite3_step(statement) != SQLITE_DONE {
            print("Error while deleting note")
            return -1
        }
        
        sqlite3_finalize(statement)
        
        return 0
    }
    
    // reads in all the notes from the database.
    // used to populate the TableView
    func getAllNotes() -> [Note] {
        connect()
        
        var result: [Note] = []
        var statement: OpaquePointer!
        
        if sqlite3_prepare_v2(database, "SELECT rowid, title, contents, date FROM notes", -1, &statement, nil) != SQLITE_OK {
            print("Could not retrieve all notes")
            return []
        }
        
        while sqlite3_step(statement) == SQLITE_ROW {
            let id = Int(sqlite3_column_int(statement, 0))
            let noteTitle = String(cString: sqlite3_column_text(statement, 1))
            let content = String(cString: sqlite3_column_text(statement, 2))
            
            // format the date
            let CustomDateFormatter = DateFormatter()
            CustomDateFormatter.dateFormat = "yyyy-MM-dd HH:mm:ss"
            let date = CustomDateFormatter.date(from: String(cString: sqlite3_column_text(statement, 3)))!
            CustomDateFormatter.locale = Locale(identifier: "de_AT")
            CustomDateFormatter.timeZone = TimeZone.current
            CustomDateFormatter.dateStyle = .short
            CustomDateFormatter.timeStyle = .short
            let dateString = CustomDateFormatter.string(from: date) + " Uhr"
            
            result.append(Note(id: id, noteTitle: noteTitle, content: content, date: dateString))
        }
        
        sqlite3_finalize(statement)
        
        return result
    }
}
