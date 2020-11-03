import UIKit

class NoteViewController: UIViewController {
    @IBOutlet var contentTextView: UITextView!
    @IBAction func deleteNote(_ sender: Any) {
        NoteManager.shared.deleteNote(note: note!)
        navigationController?.popViewController(animated: true)
    }
    
    var note: Note? = nil
    
    override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(animated)
        
        contentTextView.text = note!.content
    }
    
    override func viewWillDisappear(_ animated: Bool) {
        super.viewWillDisappear(animated)
        
        note!.content = contentTextView.text
        NoteManager.shared.saveNote(note: note!)
    }
}
