SELECT title FROM movies
JOIN
(
  (SELECT movie_id FROM people JOIN stars ON people.id = stars.person_id WHERE name = "Johnny Depp") johnny
  JOIN 
  (SELECT movie_id FROM people JOIN stars ON people.id = stars.person_id WHERE name = "Helena Bonham Carter") helena
  ON (johnny.movie_id = helena.movie_id)
) result
ON result.movie_id = movies.id;