SELECT DISTINCT people.name
FROM people
JOIN
(
  -- find movies starring Kevin Bacon
  (SELECT movie_id, name FROM people JOIN stars ON people.id = stars.person_id WHERE name = "Kevin Bacon" AND birth = "1958") kevin
  JOIN 
  -- find other actors starring in any movie
  (SELECT movie_id, person_id FROM people JOIN stars ON people.id = stars.person_id) other
  -- filter out those that do not match movies starring Kevin Bacon
  ON (kevin.movie_id = other.movie_id)
) result
ON result.person_id = people.id
WHERE people.name IS NOT "Kevin Bacon";