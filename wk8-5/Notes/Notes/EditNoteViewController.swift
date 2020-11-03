//
//  EditNoteViewController.swift
//  Notes
//
//  Created by Luka Rivic on 30.10.20.
//

import Foundation
import UIKit

class EditNoteViewController: UIViewController {
    var note: Note!
    
    @IBAction func deleteNote() {
        if NoteManager.main.deleteNote(note: note) == 0 {
            performSegue(withIdentifier: "unwindToNotesList", sender: self)
        }
    }
    
    @IBOutlet var noteContents: UITextView!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        noteContents.text = note.content
        navigationItem.title = note.noteTitle
    }
    
    override func viewWillDisappear(_ animated: Bool) {
        super.viewWillDisappear(animated)
        if note.content != noteContents.text {
            note.content = noteContents.text
            note.noteTitle = String(noteContents.text.prefix(9))
            if note.noteTitle.count > 9 { note.noteTitle += "…" }
            NoteManager.main.updateNote(note: note)
        }
    }
    
}
