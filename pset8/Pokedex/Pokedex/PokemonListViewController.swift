import UIKit

class PokemonListViewController: UITableViewController {
    var pokemon: [PokemonListResult] = []
    var filteredPokemon: [PokemonListResult] = []

    // Search
    let searchController = UISearchController(searchResultsController: nil)
    
    // Helpers:
    // Capitalize first letter of string
    func capitalize(text: String) -> String {
        return text.prefix(1).uppercased() + text.dropFirst()
    }
    
    // returns true if the search bar is empty
    var isSearchBarEmpty: Bool {
        return searchController.searchBar.text?.isEmpty ?? true
    }
    
    // returns true if filtering is acitve
    var isFiltering: Bool {
        return !isSearchBarEmpty
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        guard let url = URL(string: "https://pokeapi.co/api/v2/pokemon?limit=151") else {
            return
        }
        
        URLSession.shared.dataTask(with: url) { (data, response, error) in
            guard let data = data else {
                return
            }
            
            do {
                let entries = try JSONDecoder().decode(PokemonListResults.self, from: data)
                self.pokemon = entries.results
                DispatchQueue.main.async {
                    self.tableView.reloadData()
                }
            }
            catch let error {
                print(error)
            }
        }.resume()
        
        // Search
        navigationItem.searchController = searchController // use a separate controller to manage searching & displaying the results
        searchController.searchResultsUpdater = self // activate the search results updater from the extension below
        searchController.obscuresBackgroundDuringPresentation = false // so user can click on search result
        definesPresentationContext = true // mark the ViewController as obscured
    }
    
    override func numberOfSections(in tableView: UITableView) -> Int {
        return 1
    }
    
    override func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return isFiltering ? filteredPokemon.count : pokemon.count
    }
    
    override func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        let cell = tableView.dequeueReusableCell(withIdentifier: "PokemonCell", for: indexPath)
        
        let p: PokemonListResult = isFiltering ? filteredPokemon[indexPath.row] : pokemon[indexPath.row]
        cell.textLabel?.text = capitalize(text: p.name)
        
        return cell
    }
    
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        if segue.identifier == "ShowPokemonSegue",
                let destination = segue.destination as? PokemonViewController,
                let index = tableView.indexPathForSelectedRow?.row {
            destination.url = pokemon[index].url
        }
    }
}

// MARK: - UISearchBarDelegate
extension PokemonListViewController: UISearchResultsUpdating {
    func updateSearchResults(for searchController: UISearchController) {
        filteredPokemon = pokemon.filter {(pokemon: PokemonListResult) -> Bool in
            return pokemon.name.lowercased().contains(searchController.searchBar.text!.lowercased())
    }
        
        tableView.reloadData()
    }
}
