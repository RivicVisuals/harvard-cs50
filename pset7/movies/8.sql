SELECT name 
FROM people 
JOIN 
(
  stars JOIN movies ON movies.id = stars.movie_id
) ON people.id = stars.person_id 
WHERE movies.title="Toy Story";
