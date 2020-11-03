//
//  ViewController.swift
//  Notes
//
//  Created by Luka Rivic on 30.10.20.
//

import UIKit

class ViewController: UITableViewController {

    var notes: [Note] = []
    
    @IBAction func addNote() {
        performSegue(withIdentifier: "newNoteSegue", sender: self)
    }
    
    @IBAction func unwind(_ segue: UIStoryboardSegue) {}
    
    func reload() {
        notes = NoteManager.main.getAllNotes()
        self.tableView.reloadData()
    }
    
    override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(animated)
        
        reload()
    }

// MARK: - Set up the table
    override func numberOfSections(in tableView: UITableView) -> Int {
        return 1
    }
    
    override func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return notes.count
    }
    
    override func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        let cell = tableView.dequeueReusableCell(withIdentifier: "NoteListCell", for: indexPath)
        cell.textLabel?.text = notes[indexPath.row].noteTitle
        cell.detailTextLabel?.text = notes[indexPath.row].date
        return cell
    }
    
    override func tableView(_ tableView: UITableView, trailingSwipeActionsConfigurationForRowAt indexPath: IndexPath) -> UISwipeActionsConfiguration? {
        
        let action = UIContextualAction(style: .destructive, title: nil, handler: {
            (_, _, completionHandler) in
            if NoteManager.main.deleteNote(note: self.notes[indexPath.row]) == 0 {
                self.reload()
                completionHandler(true)
            } else {
                completionHandler(false)
            }
        })
        
        action.image = UIImage(systemName: "trash")
        
        let config = UISwipeActionsConfiguration(actions: [action])
        return config
    }
    
// MARK: - Segues
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        if segue.identifier == "newNoteSegue" {
            if let destination = segue.destination as? EditNoteViewController {
                let n = NoteManager.main.insertNote()
                reload()
                destination.note = notes[n-1] // array of notes is 0-indexed, the rowid in the database is 1-indexed
            }
        }
        
        if segue.identifier == "showNoteSegue" {
            if let destination = segue.destination as? EditNoteViewController {
                destination.note = notes[tableView.indexPathForSelectedRow!.row]
            }
        }
    }
}

