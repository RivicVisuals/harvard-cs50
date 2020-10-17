SELECT title
FROM movies 
JOIN stars ON stars.movie_id = movies.id 
JOIN people ON people.id = stars.person_id 
JOIN ratings ON movies.id = ratings.movie_id 
WHERE name = "Chadwick Boseman" 
ORDER BY ratings.rating DESC
LIMIT 5;