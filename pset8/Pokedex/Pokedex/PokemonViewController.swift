import UIKit
import Foundation

class PokemonViewController: UIViewController {
    var url: String!
    var name: String!
    var id: Int!
    var caughtStatus: Bool!

    @IBOutlet var nameLabel: UILabel!
    @IBOutlet var numberLabel: UILabel!
    @IBOutlet var type1Label: UILabel!
    @IBOutlet var type2Label: UILabel!
    @IBOutlet var sprite: UIImageView!
    @IBOutlet var descriptionText: UITextView!
    @IBOutlet var catchButton: UIBarButtonItem!
    
    @IBAction func toggleCatch() {
        catchButton.possibleTitles = ["Catch", "Add"]
        
        // invert current caughtStatus
        caughtStatus = !caughtStatus
        
        // save the state of the caughtStatus
        UserDefaults.standard.setValue(caughtStatus, forKey: name)
        
        // redraw UI
        catchButton.title = caughtStatus ? "Release" : "Catch"
        UIView.animate(withDuration: 0.4) {
            self.sprite.layer.shadowOpacity = self.caughtStatus ? 0.5 : 0
        }
    }
    
    func capitalize(text: String) -> String {
        return text.prefix(1).uppercased() + text.dropFirst()
    }

    override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(animated)

        nameLabel.text = ""
        numberLabel.text = ""
        type1Label.text = ""
        type2Label.text = ""
        sprite.image = nil
        sprite.layer.shadowColor = UIColor.systemTeal.cgColor
        sprite.layer.shadowOffset = .zero
        sprite.layer.shadowRadius = 4
        descriptionText.text = ""
        loadPokemon()
    }

    func loadPokemon() {
        URLSession.shared.dataTask(with: URL(string: url)!) { (data, response, error) in
            guard let data = data else {
                return
            }

            do {
                let result = try JSONDecoder().decode(PokemonResult.self, from: data)
                DispatchQueue.main.async {
                    self.navigationItem.title = self.capitalize(text: result.name)
                    self.nameLabel.text = self.capitalize(text: result.name)
                    self.numberLabel.text = String(format: "#%03d", result.id)
                    // store the Pokemon's ID for future reference
                    self.id = result.id
                    self.loadDescription()
                    
                    
                    // Try and get the URL of the front default sprite
                    if let spriteURL = result.sprites.front_default {
                        // if there is a URL associated, load it into our UIImageView
                        self.sprite.load(url: spriteURL)
                    }
                    
                    for typeEntry in result.types {
                        if typeEntry.slot == 1 {
                            self.type1Label.text = typeEntry.type.name
                        }
                        else if typeEntry.slot == 2 {
                            self.type2Label.text = typeEntry.type.name
                        }
                    }
                }
            }
            catch let error {
                print(error)
            }
        }.resume()
        
        // read from the map of <pokemon.name : caught?> and set the button accordingly
        caughtStatus = UserDefaults.standard.bool(forKey: name)
        catchButton.title = caughtStatus ? "Release" : "Catch"
        sprite.layer.shadowOpacity = caughtStatus ? 0.5 : 0
    }
    
    func loadDescription() {
        // construct the separate API URL for downloading the description.
        guard let id = self.id else {
            return
        }
        let descURL: String = "https://pokeapi.co/api/v2/pokemon-species/\(id)/"
        
        URLSession.shared.dataTask(with: URL(string: descURL)!) { (data, response, error) in
            guard let data = data else {
                return
            }

            do {
                let result = try JSONDecoder().decode(PokemonDescription.self, from: data)
                DispatchQueue.main.async {
                    self.descriptionText.text = result.flavor_text_entries[0].flavor_text
                        .replacingOccurrences(of: #"\f"#, with: "\n", options: .regularExpression) // get rid of the damn form feeds
                        .components(separatedBy: CharacterSet(charactersIn: "\n\r")) // sanitize line breaks from original texts
                        .joined(separator: " ")
                }
            }
            catch let error {
                print(error)
            }
        }.resume()
    }
}

// MARK: - UIImageView
// extends UIImageView with support for downloading an image from the passed URL,
// in a background thread.
// Code snippet found at: https://www.hackingwithswift.com/example-code/uikit/how-to-load-a-remote-image-url-into-uiimageview
extension UIImageView {
    func load(url: URL) {
        DispatchQueue.global().async { [weak self] in
            if let data = try? Data(contentsOf: url) {
                if let image = UIImage(data: data) {
                    DispatchQueue.main.async {
                        self?.image = image
                    }
                }
            }
        }
    }
}
