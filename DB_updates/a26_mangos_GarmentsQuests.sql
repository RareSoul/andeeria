-- Garments of .... (priest quests)
-- Deathguard Kel || Grunt Kor'ja || Mountaineer Dolf || Guard Roberts || Sentinel Shaya
UPDATE creature_template SET unit_flags = unit_flags|8 WHERE entry IN (12428,12430,12427,12423,12429);