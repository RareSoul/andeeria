SET @WS_MAP = 489;
SET @ALLIANCE_SPIRIT = 13116;
SET @HORDE_SPIRIT = 13117;
SET @VERY_LONG = 60*60*24;
SET @BUFF_RESPAWN = 180;

-- DELETE FROM gameobject WHERE map=@WS_MAP;
-- DELETE FROM creature WHERE map=@WS_MAP;
-- DELETE FROM gameobject_battleground;
-- DELETE FROM creature_battleground;
DELETE FROM gameobject_battleground WHERE guid IN(SELECT guid FROM gameobject WHERE map=@WS_MAP);
DELETE FROM creature_battleground WHERE guid IN(SELECT guid FROM creature WHERE map=@WS_MAP);
DELETE FROM gameobject WHERE map=@WS_MAP;
DELETE FROM creature WHERE map=@WS_MAP;

-- alliance flag
INSERT INTO gameobject
(id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES
(179830,@WS_MAP,1540.423,1481.325,351.8284,3.089233,@VERY_LONG);
INSERT INTO gameobject_battleground (guid, event1, event2) SELECT guid, 0, 0 from
gameobject WHERE map=@WS_MAP AND id IN(179830) LIMIT 1;

-- horde flag
INSERT INTO gameobject
(id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES
(179831,@WS_MAP,916.0226,1434.405,345.413,0.01745329,@VERY_LONG);
INSERT INTO gameobject_battleground (guid, event1, event2) SELECT guid, 1, 0 from
gameobject WHERE map=@WS_MAP AND id IN(179831) LIMIT 1;


-- buffs
-- .. are static and don't need special handling of bg-code
INSERT INTO gameobject
(id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES
(179871,@WS_MAP,1449.93,1470.71,342.6346,-1.64061,@BUFF_RESPAWN);
INSERT INTO gameobject
(id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES
(179871,@WS_MAP,1005.171,1447.946,335.9032,1.64061,@BUFF_RESPAWN);
INSERT INTO gameobject
(id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES
(179904,@WS_MAP,1317.506,1550.851,313.2344,-0.2617996,@BUFF_RESPAWN);
INSERT INTO gameobject
(id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES
(179904,@WS_MAP,1110.451,1353.656,316.5181,-0.6806787,@BUFF_RESPAWN);
INSERT INTO gameobject
(id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES
(179905,@WS_MAP,1320.09,1378.79,314.7532,1.186824,@BUFF_RESPAWN);
INSERT INTO gameobject
(id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES
(179905,@WS_MAP,1139.688,1560.288,306.8432,-2.443461,@BUFF_RESPAWN);

-- alliance gates
INSERT INTO gameobject
(id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES
(179918,@WS_MAP,1503.335,1493.466,352.1888,3.115414,@VERY_LONG);
INSERT INTO gameobject
(id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES
(179919,@WS_MAP,1492.478,1457.912,342.9689,3.115414,@VERY_LONG);
INSERT INTO gameobject
(id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES
(179920,@WS_MAP,1468.503,1494.357,351.8618,3.115414,@VERY_LONG);
INSERT INTO gameobject
(id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES
(179921,@WS_MAP,1471.555,1458.778,362.6332,3.115414,@VERY_LONG);
-- those objects are invisible - not sure, if they are realy needed
-- INSERT INTO gameobject
-- (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES
-- (179922,@WS_MAP,1492.347,1458.34,342.3712,-0.03490669,@VERY_LONG);
-- INSERT INTO gameobject
-- (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES
-- (179922,@WS_MAP,1503.466,1493.367,351.7352,-0.03490669,@VERY_LONG);

-- horde gates
INSERT INTO gameobject
(id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES
(179916,@WS_MAP,949.1663,1423.772,345.6241,-0.5756807,@VERY_LONG);
INSERT INTO gameobject
(id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES
(179917,@WS_MAP,953.0507,1459.842,340.6526,-1.99662,@VERY_LONG);
-- those objects are invisible - not sure, if they are realy needed
-- INSERT INTO gameobject
-- (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES
-- (180322,@WS_MAP,949.9523,1422.751,344.9273,0.0,@VERY_LONG);
-- INSERT INTO gameobject
-- (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES
-- (180322,@WS_MAP,950.7952,1459.583,342.1523,0.05235988,@VERY_LONG);

-- i just add all doors to alliance-doorevent (cause on blizz doors open at the
-- same time)
INSERT INTO gameobject_battleground (guid, event1, event2) SELECT guid, 254, 0 from
gameobject WHERE map=@WS_MAP AND id IN(179918, 179919, 179920, 179921, 179922,
179916, 179917, 180322);

-- spiritguides
INSERT INTO
creature(id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES(@ALLIANCE_SPIRIT,@WS_MAP,1415.33, 1554.79, 343.156,3.14159,@VERY_LONG);
INSERT INTO
creature(id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES(@HORDE_SPIRIT,@WS_MAP,1029.14, 1387.49, 340.836,3.14159,@VERY_LONG);

INSERT INTO creature_battleground (guid, event1, event2) SELECT guid, 2, 0 from
creature WHERE map=@WS_MAP AND id IN(@ALLIANCE_SPIRIT, @HORDE_SPIRIT);


-- cleanup
UPDATE gameobject SET animprogress=100, state=1 WHERE map=@WS_MAP;
SET @AB_MAP = 529;
SET @ALLIANCE_SPIRIT = 13116;
SET @HORDE_SPIRIT = 13117;
SET @VERY_LONG = 60*60*24;

-- clean up
DELETE FROM gameobject_battleground WHERE guid IN(SELECT guid FROM gameobject WHERE map=@AB_MAP);
DELETE FROM creature_battleground WHERE guid IN(SELECT guid FROM creature WHERE map=@AB_MAP);
DELETE FROM gameobject WHERE map=@AB_MAP;
DELETE FROM creature WHERE map=@AB_MAP;

-- stables
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (180058,@AB_MAP,1166.785,1200.132,-56.70859,0.9075713,@VERY_LONG);
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (180059,@AB_MAP,1166.785,1200.132,-56.70859,0.9075713,@VERY_LONG);
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (180060,@AB_MAP,1166.785,1200.132,-56.70859,0.9075713,@VERY_LONG);
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (180061,@AB_MAP,1166.785,1200.132,-56.70859,0.9075713,@VERY_LONG);
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (180100,@AB_MAP,1166.785,1200.132,-56.70859,0.9075713,@VERY_LONG);
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (180101,@AB_MAP,1166.785,1200.132,-56.70859,0.9075713,@VERY_LONG);
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (180102,@AB_MAP,1166.785,1200.132,-56.70859,0.9075713,@VERY_LONG);
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (180087,@AB_MAP,1166.785,1200.132,-56.70859,0.9075713,@VERY_LONG);
INSERT INTO gameobject_battleground(guid,event1, event2) SELECT guid, 0, 3 FROM gameobject WHERE id IN(180058, 180100) ORDER BY guid DESC  LIMIT 2; -- alliance
INSERT INTO gameobject_battleground(guid,event1, event2) SELECT guid, 0, 1 FROM gameobject WHERE id IN(180059, 180102) ORDER BY guid DESC LIMIT 2; -- alliance contested
INSERT INTO gameobject_battleground(guid,event1, event2) SELECT guid, 0, 0 FROM gameobject WHERE id IN(180087) ORDER BY guid DESC LIMIT 1; -- neutral
INSERT INTO gameobject_battleground(guid,event1, event2) SELECT guid, 0, 4 FROM gameobject WHERE id IN(180060, 180101) ORDER BY guid DESC LIMIT 2; -- horde
INSERT INTO gameobject_battleground(guid,event1, event2) SELECT guid, 0, 2 FROM gameobject WHERE id IN(180061) ORDER BY guid DESC LIMIT 1; -- horde contested

-- blacksmith
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (180058,@AB_MAP,977.0156,1046.616,-44.80923,-2.600541,@VERY_LONG);
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (180059,@AB_MAP,977.0156,1046.616,-44.80923,-2.600541,@VERY_LONG);
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (180060,@AB_MAP,977.0156,1046.616,-44.80923,-2.600541,@VERY_LONG);
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (180061,@AB_MAP,977.0156,1046.616,-44.80923,-2.600541,@VERY_LONG);
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (180100,@AB_MAP,977.0156,1046.616,-44.80923,-2.600541,@VERY_LONG);
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (180101,@AB_MAP,977.0156,1046.616,-44.80923,-2.600541,@VERY_LONG);
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (180102,@AB_MAP,977.0156,1046.616,-44.80923,-2.600541,@VERY_LONG);
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (180088,@AB_MAP,977.0156,1046.616,-44.80923,-2.600541,@VERY_LONG);
INSERT INTO gameobject_battleground(guid,event1, event2) SELECT guid, 1, 3 FROM gameobject WHERE id IN(180058, 180100) ORDER BY guid DESC LIMIT 2; -- alliance
INSERT INTO gameobject_battleground(guid,event1, event2) SELECT guid, 1, 1 FROM gameobject WHERE id IN(180059, 180102) ORDER BY guid DESC LIMIT 2; -- alliance contested
INSERT INTO gameobject_battleground(guid,event1, event2) SELECT guid, 1, 0 FROM gameobject WHERE id IN(180088) ORDER BY guid DESC LIMIT 1; -- neutral
INSERT INTO gameobject_battleground(guid,event1, event2) SELECT guid, 1, 4 FROM gameobject WHERE id IN(180060, 180101) ORDER BY guid DESC LIMIT 2; -- horde
INSERT INTO gameobject_battleground(guid,event1, event2) SELECT guid, 1, 2 FROM gameobject WHERE id IN(180061) ORDER BY guid DESC LIMIT 1; -- horde contested

-- farm
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (180058,@AB_MAP,806.1821,874.2723,-55.99371,-2.303835,@VERY_LONG);
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (180059,@AB_MAP,806.1821,874.2723,-55.99371,-2.303835,@VERY_LONG);
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (180060,@AB_MAP,806.1821,874.2723,-55.99371,-2.303835,@VERY_LONG);
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (180061,@AB_MAP,806.1821,874.2723,-55.99371,-2.303835,@VERY_LONG);
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (180100,@AB_MAP,806.1821,874.2723,-55.99371,-2.303835,@VERY_LONG);
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (180101,@AB_MAP,806.1821,874.2723,-55.99371,-2.303835,@VERY_LONG);
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (180102,@AB_MAP,806.1821,874.2723,-55.99371,-2.303835,@VERY_LONG);
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (180089,@AB_MAP,806.1821,874.2723,-55.99371,-2.303835,@VERY_LONG);
INSERT INTO gameobject_battleground(guid,event1, event2) SELECT guid, 2, 3 FROM gameobject WHERE id IN(180058, 180100) ORDER BY guid DESC LIMIT 2; -- alliance
INSERT INTO gameobject_battleground(guid,event1, event2) SELECT guid, 2, 1 FROM gameobject WHERE id IN(180059, 180102) ORDER BY guid DESC LIMIT 2; -- alliance contested
INSERT INTO gameobject_battleground(guid,event1, event2) SELECT guid, 2, 0 FROM gameobject WHERE id IN(180089) ORDER BY guid DESC LIMIT 1; -- neutral
INSERT INTO gameobject_battleground(guid,event1, event2) SELECT guid, 2, 4 FROM gameobject WHERE id IN(180060, 180101) ORDER BY guid DESC LIMIT 2; -- horde
INSERT INTO gameobject_battleground(guid,event1, event2) SELECT guid, 2, 2 FROM gameobject WHERE id IN(180061) ORDER BY guid DESC LIMIT 1; -- horde contested

-- lumber mill
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (180058,@AB_MAP,856.1419,1148.902,11.18469,-2.303835,@VERY_LONG);
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (180059,@AB_MAP,856.1419,1148.902,11.18469,-2.303835,@VERY_LONG);
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (180060,@AB_MAP,856.1419,1148.902,11.18469,-2.303835,@VERY_LONG);
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (180061,@AB_MAP,856.1419,1148.902,11.18469,-2.303835,@VERY_LONG);
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (180100,@AB_MAP,856.1419,1148.902,11.18469,-2.303835,@VERY_LONG);
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (180101,@AB_MAP,856.1419,1148.902,11.18469,-2.303835,@VERY_LONG);
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (180102,@AB_MAP,856.1419,1148.902,11.18469,-2.303835,@VERY_LONG);
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (180090,@AB_MAP,856.1419,1148.902,11.18469,-2.303835,@VERY_LONG);
INSERT INTO gameobject_battleground(guid,event1, event2) SELECT guid, 3, 3 FROM gameobject WHERE id IN(180058, 180100) ORDER BY guid DESC LIMIT 2; -- alliance
INSERT INTO gameobject_battleground(guid,event1, event2) SELECT guid, 3, 1 FROM gameobject WHERE id IN(180059, 180102) ORDER BY guid DESC LIMIT 2; -- alliance contested
INSERT INTO gameobject_battleground(guid,event1, event2) SELECT guid, 3, 0 FROM gameobject WHERE id IN(180090) ORDER BY guid DESC LIMIT 1; -- neutral
INSERT INTO gameobject_battleground(guid,event1, event2) SELECT guid, 3, 4 FROM gameobject WHERE id IN(180060, 180101) ORDER BY guid DESC LIMIT 2; -- horde
INSERT INTO gameobject_battleground(guid,event1, event2) SELECT guid, 3, 2 FROM gameobject WHERE id IN(180061) ORDER BY guid DESC LIMIT 1; -- horde contested

-- gold mine
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (180058,@AB_MAP,1146.923,848.1782,-110.917,-0.7330382,@VERY_LONG);
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (180059,@AB_MAP,1146.923,848.1782,-110.917,-0.7330382,@VERY_LONG);
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (180060,@AB_MAP,1146.923,848.1782,-110.917,-0.7330382,@VERY_LONG);
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (180061,@AB_MAP,1146.923,848.1782,-110.917,-0.7330382,@VERY_LONG);
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (180100,@AB_MAP,1146.923,848.1782,-110.917,-0.7330382,@VERY_LONG);
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (180101,@AB_MAP,1146.923,848.1782,-110.917,-0.7330382,@VERY_LONG);
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (180102,@AB_MAP,1146.923,848.1782,-110.917,-0.7330382,@VERY_LONG);
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (180091,@AB_MAP,1146.923,848.1782,-110.917,-0.7330382,@VERY_LONG);
INSERT INTO gameobject_battleground(guid,event1, event2) SELECT guid, 4, 3 FROM gameobject WHERE id IN(180058, 180100) ORDER BY guid DESC LIMIT 2; -- alliance
INSERT INTO gameobject_battleground(guid,event1, event2) SELECT guid, 4, 1 FROM gameobject WHERE id IN(180059, 180102) ORDER BY guid DESC LIMIT 2; -- alliance contested
INSERT INTO gameobject_battleground(guid,event1, event2) SELECT guid, 4, 0 FROM gameobject WHERE id IN(180091) ORDER BY guid DESC LIMIT 1; -- neutral
INSERT INTO gameobject_battleground(guid,event1, event2) SELECT guid, 4, 4 FROM gameobject WHERE id IN(180060, 180101) ORDER BY guid DESC LIMIT 2; -- horde
INSERT INTO gameobject_battleground(guid,event1, event2) SELECT guid, 4, 2 FROM gameobject WHERE id IN(180061) ORDER BY guid DESC LIMIT 1; -- horde contested


-- doors
-- alliance
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (180255, @AB_MAP,1284.597, 1281.167, -15.97792, 0.7068594, @VERY_LONG);
INSERT INTO gameobject_battleground(guid,event1, event2) SELECT guid, 254, 0 FROM gameobject WHERE id IN(180255) ORDER BY guid DESC LIMIT 1;
-- horde
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (180256,@AB_MAP, 708.0903, 708.4479, -17.8342, -2.391099, @VERY_LONG);
INSERT INTO gameobject_battleground(guid,event1, event2) SELECT guid, 254, 0 FROM gameobject WHERE id IN(180256) ORDER BY guid DESC LIMIT 1;


-- spiritguides
-- stables
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES(@ALLIANCE_SPIRIT,@AB_MAP,1200.03, 1171.09, -56.47, 5.15,@VERY_LONG);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES(@HORDE_SPIRIT,@AB_MAP,1200.03, 1171.09, -56.47, 5.15,@VERY_LONG);
INSERT INTO creature_battleground (guid,event1,event2) SELECT guid, 0, 3 FROM creature WHERE id IN(@ALLIANCE_SPIRIT) ORDER BY guid DESC LIMIT 1;
INSERT INTO creature_battleground (guid,event1,event2) SELECT guid, 0, 4 FROM creature WHERE id IN(@HORDE_SPIRIT) ORDER BY guid DESC LIMIT 1;
-- blacksmith
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES(@ALLIANCE_SPIRIT,@AB_MAP,1017.43, 960.61, -42.95, 4.88,@VERY_LONG);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES(@HORDE_SPIRIT,@AB_MAP,1017.43, 960.61, -42.95, 4.88,@VERY_LONG);
INSERT INTO creature_battleground (guid,event1,event2) SELECT guid, 1, 3 FROM creature WHERE id IN(@ALLIANCE_SPIRIT) ORDER BY guid DESC LIMIT 1;
INSERT INTO creature_battleground (guid,event1,event2) SELECT guid, 1, 4 FROM creature WHERE id IN(@HORDE_SPIRIT) ORDER BY guid DESC LIMIT 1;
-- arm
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES(@ALLIANCE_SPIRIT,@AB_MAP,833.00, 793.00, -57.25, 5.27,@VERY_LONG);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES(@HORDE_SPIRIT,@AB_MAP,833.00, 793.00, -57.25, 5.27,@VERY_LONG);
INSERT INTO creature_battleground (guid,event1,event2) SELECT guid, 2, 3 FROM creature WHERE id IN(@ALLIANCE_SPIRIT) ORDER BY guid DESC LIMIT 1;
INSERT INTO creature_battleground (guid,event1,event2) SELECT guid, 2, 4 FROM creature WHERE id IN(@HORDE_SPIRIT) ORDER BY guid DESC LIMIT 1;
-- lumber mill
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES(@ALLIANCE_SPIRIT,@AB_MAP,775.17, 1206.40, 15.79, 1.90,@VERY_LONG);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES(@HORDE_SPIRIT,@AB_MAP,775.17, 1206.40, 15.79, 1.90,@VERY_LONG);
INSERT INTO creature_battleground (guid,event1,event2) SELECT guid, 3, 3 FROM creature WHERE id IN(@ALLIANCE_SPIRIT) ORDER BY guid DESC LIMIT 1;
INSERT INTO creature_battleground (guid,event1,event2) SELECT guid, 3, 4 FROM creature WHERE id IN(@HORDE_SPIRIT) ORDER BY guid DESC LIMIT 1;
-- gold mine
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES(@ALLIANCE_SPIRIT,@AB_MAP,1207.48, 787.00, -83.36, 5.51,@VERY_LONG);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES(@HORDE_SPIRIT,@AB_MAP,1207.48, 787.00, -83.36, 5.51,@VERY_LONG);
INSERT INTO creature_battleground (guid,event1,event2) SELECT guid, 4, 3 FROM creature WHERE id IN(@ALLIANCE_SPIRIT) ORDER BY guid DESC LIMIT 1;
INSERT INTO creature_battleground (guid,event1,event2) SELECT guid, 4, 4 FROM creature WHERE id IN(@HORDE_SPIRIT) ORDER BY guid DESC LIMIT 1;
-- starting base
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES(@ALLIANCE_SPIRIT,@AB_MAP,1354.05, 1275.48, -11.30, 4.77,@VERY_LONG);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES(@HORDE_SPIRIT,@AB_MAP,714.61, 646.15, -10.87, 4.34,@VERY_LONG);



UPDATE gameobject SET animprogress=100, state=1 WHERE map=@AB_MAP;
SET @EY_MAP = 566;
SET @ALLIANCE_SPIRIT = 13116;
SET @HORDE_SPIRIT = 13117;
SET @VERY_LONG = 60*60*24;

-- clean up
DELETE FROM gameobject_battleground WHERE guid IN(SELECT guid FROM gameobject WHERE map=@EY_MAP);
DELETE FROM creature_battleground WHERE guid IN(SELECT guid FROM creature WHERE map=@EY_MAP);
DELETE FROM gameobject WHERE map=@EY_MAP;
DELETE FROM creature WHERE map=@EY_MAP;


-- banners (alliance)
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (184381,@EY_MAP,2057.46,1735.07,1187.91,-0.925024,@VERY_LONG);
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (184381,@EY_MAP,2032.25,1729.53,1190.33,1.8675,@VERY_LONG);
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (184381,@EY_MAP,2092.35,1775.46,1187.08,-0.401426,@VERY_LONG);
INSERT INTO gameobject_battleground(guid,event1, event2) SELECT guid, 0, 0 FROM gameobject WHERE id IN(184381) ORDER BY guid DESC  LIMIT 3;

INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (184381,@EY_MAP,2047.19,1349.19,1189.0,-1.62316,@VERY_LONG);
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (184381,@EY_MAP,2074.32,1385.78,1194.72,0.488692,@VERY_LONG);
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (184381,@EY_MAP,2025.13,1386.12,1192.74,2.3911,@VERY_LONG);
INSERT INTO gameobject_battleground(guid,event1, event2) SELECT guid, 1, 0 FROM gameobject WHERE id IN(184381) ORDER BY guid DESC  LIMIT 3;

INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (184381,@EY_MAP,2276.8,1400.41,1196.33,2.44346,@VERY_LONG);
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (184381,@EY_MAP,2305.78,1404.56,1199.38,1.74533,@VERY_LONG);
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (184381,@EY_MAP,2245.4,1366.41,1195.28,2.21657,@VERY_LONG);
INSERT INTO gameobject_battleground(guid,event1, event2) SELECT guid, 2, 0 FROM gameobject WHERE id IN(184381) ORDER BY guid DESC  LIMIT 3;

INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (184381,@EY_MAP,2270.84,1784.08,1186.76,2.42601,@VERY_LONG);
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (184381,@EY_MAP,2269.13,1737.7,1186.66,0.994838,@VERY_LONG);
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (184381,@EY_MAP,2300.86,1741.25,1187.7,-0.785398,@VERY_LONG);
INSERT INTO gameobject_battleground(guid,event1, event2) SELECT guid, 3, 0 FROM gameobject WHERE id IN(184381) ORDER BY guid DESC  LIMIT 3;

-- banners (horde)
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (184380,@EY_MAP,2057.46,1735.07,1187.91,-0.925024,@VERY_LONG);
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (184380,@EY_MAP,2032.25,1729.53,1190.33,1.8675,@VERY_LONG);
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (184380,@EY_MAP,2092.35,1775.46,1187.08,-0.401426,@VERY_LONG);
INSERT INTO gameobject_battleground(guid,event1, event2) SELECT guid, 0, 1 FROM gameobject WHERE id IN(184380) ORDER BY guid DESC  LIMIT 3;

INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (184380,@EY_MAP,2047.19,1349.19,1189.0,-1.62316,@VERY_LONG);
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (184380,@EY_MAP,2074.32,1385.78,1194.72,0.488692,@VERY_LONG);
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (184380,@EY_MAP,2025.13,1386.12,1192.74,2.3911,@VERY_LONG);
INSERT INTO gameobject_battleground(guid,event1, event2) SELECT guid, 1, 1 FROM gameobject WHERE id IN(184380) ORDER BY guid DESC  LIMIT 3;

INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (184380,@EY_MAP,2276.8,1400.41,1196.33,2.44346,@VERY_LONG);
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (184380,@EY_MAP,2305.78,1404.56,1199.38,1.74533,@VERY_LONG);
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (184380,@EY_MAP,2245.4,1366.41,1195.28,2.21657,@VERY_LONG);
INSERT INTO gameobject_battleground(guid,event1, event2) SELECT guid, 2, 1 FROM gameobject WHERE id IN(184380) ORDER BY guid DESC  LIMIT 3;

INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (184380,@EY_MAP,2270.84,1784.08,1186.76,2.42601,@VERY_LONG);
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (184380,@EY_MAP,2269.13,1737.7,1186.66,0.994838,@VERY_LONG);
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (184380,@EY_MAP,2300.86,1741.25,1187.7,-0.785398,@VERY_LONG);
INSERT INTO gameobject_battleground(guid,event1, event2) SELECT guid, 3, 1 FROM gameobject WHERE id IN(184380) ORDER BY guid DESC  LIMIT 3;

-- banners (neutral)
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (184382,@EY_MAP,2057.46,1735.07,1187.91,-0.925024,@VERY_LONG);
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (184382,@EY_MAP,2032.25,1729.53,1190.33,1.8675,@VERY_LONG);
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (184382,@EY_MAP,2092.35,1775.46,1187.08,-0.401426,@VERY_LONG);
INSERT INTO gameobject_battleground(guid,event1, event2) SELECT guid, 0, 2 FROM gameobject WHERE id IN(184382) ORDER BY guid DESC  LIMIT 3;

INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (184382,@EY_MAP,2047.19,1349.19,1189.0,-1.62316,@VERY_LONG);
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (184382,@EY_MAP,2074.32,1385.78,1194.72,0.488692,@VERY_LONG);
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (184382,@EY_MAP,2025.13,1386.12,1192.74,2.3911,@VERY_LONG);
INSERT INTO gameobject_battleground(guid,event1, event2) SELECT guid, 1, 2 FROM gameobject WHERE id IN(184382) ORDER BY guid DESC  LIMIT 3;

INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (184382,@EY_MAP,2276.8,1400.41,1196.33,2.44346,@VERY_LONG);
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (184382,@EY_MAP,2305.78,1404.56,1199.38,1.74533,@VERY_LONG);
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (184382,@EY_MAP,2245.4,1366.41,1195.28,2.21657,@VERY_LONG);
INSERT INTO gameobject_battleground(guid,event1, event2) SELECT guid, 2, 2 FROM gameobject WHERE id IN(184382) ORDER BY guid DESC  LIMIT 3;

INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (184382,@EY_MAP,2270.84,1784.08,1186.76,2.42601,@VERY_LONG);
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (184382,@EY_MAP,2269.13,1737.7,1186.66,0.994838,@VERY_LONG);
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (184382,@EY_MAP,2300.86,1741.25,1187.7,-0.785398,@VERY_LONG);
INSERT INTO gameobject_battleground(guid,event1, event2) SELECT guid, 3, 2 FROM gameobject WHERE id IN(184382) ORDER BY guid DESC  LIMIT 3;

-- flags
-- this is the flag which can be captured
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (184141,@EY_MAP,2174.782227,1569.054688,1160.361938,-1.448624,@VERY_LONG);
INSERT INTO gameobject_battleground(guid,event1, event2) SELECT guid, 4, 4 FROM gameobject WHERE id IN(184141) ORDER BY guid DESC  LIMIT 1;

-- those are spawned after capturing at the node..
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (184493,@EY_MAP,2044.28,1729.68,1189.96,-0.017453,@VERY_LONG);
INSERT INTO gameobject_battleground(guid,event1, event2) SELECT guid, 4, 0 FROM gameobject ORDER BY guid DESC  LIMIT 1; -- sorry to lazy for copy&paste
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (184493,@EY_MAP,2048.83,1393.65,1194.49,0.20944,@VERY_LONG);
INSERT INTO gameobject_battleground(guid,event1, event2) SELECT guid, 4, 1 FROM gameobject ORDER BY guid DESC  LIMIT 1; -- sorry to lazy for copy&paste
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (184493,@EY_MAP,2286.56,1402.36,1197.11,3.72381,@VERY_LONG);
INSERT INTO gameobject_battleground(guid,event1, event2) SELECT guid, 4, 2 FROM gameobject ORDER BY guid DESC  LIMIT 1; -- sorry to lazy for copy&paste
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (184493,@EY_MAP,2284.48,1731.23,1189.99,2.89725,@VERY_LONG);
INSERT INTO gameobject_battleground(guid,event1, event2) SELECT guid, 4, 3 FROM gameobject ORDER BY guid DESC  LIMIT 1; -- sorry to lazy for copy&paste

-- tower cap
-- for me it looks like those are static
-- TODO: what's this?
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (184081,@EY_MAP,2024.600708,1742.819580,1195.157715,2.443461,@VERY_LONG);
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (184080,@EY_MAP,2050.493164,1372.235962,1194.563477,1.710423,@VERY_LONG);
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (184083,@EY_MAP,2301.010498,1386.931641,1197.183472,1.570796,@VERY_LONG);
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (184082,@EY_MAP,2282.121582,1760.006958,1189.707153,1.919862,@VERY_LONG);



-- doors
-- alliance
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (184719,@EY_MAP,2527.6,1596.91,1262.13,-3.12414,@VERY_LONG);
INSERT INTO gameobject_battleground(guid,event1,event2) SELECT guid, 254, 0 FROM gameobject ORDER BY guid DESC  LIMIT 1; -- sorry to lazy for copy&paste
-- horde
INSERT INTO gameobject (id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (184720,@EY_MAP,1803.21,1539.49,1261.09,3.14159,@VERY_LONG);
INSERT INTO gameobject_battleground(guid,event1,event2) SELECT guid, 254, 0 FROM gameobject ORDER BY guid DESC  LIMIT 1; -- sorry to lazy for copy&paste


-- spiritguides.. all orientation values are wrong
-- Graveyard (Alliance Start)
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES(@ALLIANCE_SPIRIT,@EY_MAP,2523.686, 1596.597, 1269.348,3.14159,@VERY_LONG);
-- Graveyard (Horde Start)
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES(@HORDE_SPIRIT,@EY_MAP,1807.736, 1539.416, 1267.627,3.14159,@VERY_LONG);

-- Graveyard (Felreaver)
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES(@ALLIANCE_SPIRIT,@EY_MAP,2013.062, 1677.238, 1182.126,3.14159,@VERY_LONG);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES(@HORDE_SPIRIT,@EY_MAP,2013.062, 1677.238, 1182.126,3.14159,@VERY_LONG);
INSERT INTO creature_battleground (guid,event1,event2) SELECT guid, 0, 0 FROM creature WHERE id IN(@ALLIANCE_SPIRIT) ORDER BY guid DESC LIMIT 1;
INSERT INTO creature_battleground (guid,event1,event2) SELECT guid, 0, 1 FROM creature WHERE id IN(@HORDE_SPIRIT) ORDER BY guid DESC LIMIT 1;

-- Graveyard (BE Tower)
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES(@ALLIANCE_SPIRIT,@EY_MAP,2012.403, 1455.412, 1172.202,3.14159,@VERY_LONG);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES(@HORDE_SPIRIT,@EY_MAP,2012.403, 1455.412, 1172.202,3.14159,@VERY_LONG);
INSERT INTO creature_battleground (guid,event1,event2) SELECT guid, 1, 0 FROM creature WHERE id IN(@ALLIANCE_SPIRIT) ORDER BY guid DESC LIMIT 1;
INSERT INTO creature_battleground (guid,event1,event2) SELECT guid, 1, 1 FROM creature WHERE id IN(@HORDE_SPIRIT) ORDER BY guid DESC LIMIT 1;

-- Graveyard (Draenei Tower)
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES(@ALLIANCE_SPIRIT,@EY_MAP,2351.785, 1455.399, 1185.333,3.14159,@VERY_LONG);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES(@HORDE_SPIRIT,@EY_MAP,2351.785, 1455.399, 1185.333,3.14159,@VERY_LONG);
INSERT INTO creature_battleground (guid,event1,event2) SELECT guid, 2, 0 FROM creature WHERE id IN(@ALLIANCE_SPIRIT) ORDER BY guid DESC LIMIT 1;
INSERT INTO creature_battleground (guid,event1,event2) SELECT guid, 2, 1 FROM creature WHERE id IN(@HORDE_SPIRIT) ORDER BY guid DESC LIMIT 1;

-- Graveyard (Human Tower)
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES(@ALLIANCE_SPIRIT,@EY_MAP,2355.298, 1683.714, 1173.154,3.14159,@VERY_LONG);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES(@HORDE_SPIRIT,@EY_MAP,2355.298, 1683.714, 1173.154,3.14159,@VERY_LONG);
INSERT INTO creature_battleground (guid,event1,event2) SELECT guid, 3, 0 FROM creature WHERE id IN(@ALLIANCE_SPIRIT) ORDER BY guid DESC LIMIT 1;
INSERT INTO creature_battleground (guid,event1,event2) SELECT guid, 3, 1 FROM creature WHERE id IN(@HORDE_SPIRIT) ORDER BY guid DESC LIMIT 1;



UPDATE gameobject SET animprogress=100, state=1 WHERE map=@EY_MAP;
SET @NA_MAP = 559;
SET @BE_MAP = 562;
SET @RL_MAP = 572;
SET @ALLIANCE_SPIRIT = 13116;
SET @HORDE_SPIRIT = 13117;
SET @VERY_LONG = 60*60*24;
SET @BUFF_SPAWN = 120; -- found this value in bg-code

-- clean up
DELETE FROM gameobject_battleground WHERE guid IN(SELECT guid FROM gameobject WHERE map IN(@NA_MAP, @BE_MAP, @RL_MAP));
DELETE FROM creature_battleground WHERE guid IN(SELECT guid FROM creature WHERE map IN(@NA_MAP, @BE_MAP, @RL_MAP));
DELETE FROM gameobject WHERE map IN(@NA_MAP, @BE_MAP, @RL_MAP);
DELETE FROM creature WHERE map IN(@NA_MAP, @BE_MAP, @RL_MAP);


-- na
-- gates
INSERT INTO gameobject(id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (183978,@NA_MAP,4031.854,2966.833,12.6462,-2.648788,@VERY_LONG);
INSERT INTO gameobject(id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (183980,@NA_MAP,4081.179,2874.97,12.39171,0.4928045,@VERY_LONG);
INSERT INTO gameobject_battleground(guid,event1, event2) SELECT guid, 254, 0
FROM gameobject WHERE id IN(183978,183980) ORDER BY guid DESC LIMIT 2;
-- those doors shouldn't open
INSERT INTO gameobject(id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (183977,@NA_MAP,4023.709,2981.777,10.70117,-2.648788,@VERY_LONG);
INSERT INTO gameobject(id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (183979,@NA_MAP,4090.064,2858.438,10.23631,0.4928045,@VERY_LONG);


-- buffs (getting spawned 60seconds after start - so event needed)
INSERT INTO gameobject(id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (184663,@NA_MAP,4009.189941,2895.250000,13.052700,-1.448624,@BUFF_SPAWN);
INSERT INTO gameobject(id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (184664,@NA_MAP,4103.330078,2946.350098,13.051300,-0.06981307,@BUFF_SPAWN);
INSERT INTO gameobject_battleground(guid,event1, event2) SELECT guid, 253, 0
FROM gameobject WHERE id IN(184663,184664) ORDER BY guid DESC LIMIT 2;


-- be
-- gates
INSERT INTO gameobject(id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (183971,@BE_MAP,6287.277,282.1877,3.810925,-2.260201,@VERY_LONG);
INSERT INTO gameobject(id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (183973,@BE_MAP,6189.546,241.7099,3.101481,0.8813917,@VERY_LONG);
INSERT INTO gameobject_battleground(guid,event1, event2) SELECT guid, 254, 0
FROM gameobject WHERE id IN(183971,183973) ORDER BY guid DESC LIMIT 2;

INSERT INTO gameobject(id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (183970,@BE_MAP,6299.116,296.5494,3.308032,0.8813917,@VERY_LONG);
INSERT INTO gameobject(id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (183972,@BE_MAP,6177.708,227.3481,3.604374,-2.260201,@VERY_LONG);

-- buffs (getting spawned 60seconds after start - so event needed)
INSERT INTO gameobject(id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (184663,@BE_MAP,6249.042,275.3239,11.22033,-1.448624,@BUFF_SPAWN);
INSERT INTO gameobject(id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (184664,@BE_MAP,6228.26,249.566,11.21812,-0.06981307,@BUFF_SPAWN);
INSERT INTO gameobject_battleground(guid,event1, event2) SELECT guid, 253, 0
FROM gameobject WHERE id IN(184663,184664) ORDER BY guid DESC LIMIT 2;


-- rl
-- gates
INSERT INTO gameobject(id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (185918,@RL_MAP,1293.561,1601.938,31.60557,-1.457349,@VERY_LONG);
INSERT INTO gameobject(id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (185917,@RL_MAP,1278.648,1730.557,31.60557,1.684245,@VERY_LONG);
INSERT INTO gameobject_battleground(guid,event1, event2) SELECT guid, 254, 0
FROM gameobject WHERE id IN(185918,185917) ORDER BY guid DESC LIMIT 2;

-- buffs (getting spawned 60seconds after start - so event needed)
INSERT INTO gameobject(id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (184663,@RL_MAP,1328.719971,1632.719971,36.730400,-1.448624,@BUFF_SPAWN);
INSERT INTO gameobject(id,map,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (184664,@RL_MAP,1243.300049,1699.170044,34.872601,-0.06981307,@BUFF_SPAWN);
INSERT INTO gameobject_battleground(guid,event1, event2) SELECT guid, 253, 0
FROM gameobject WHERE id IN(184663,184664) ORDER BY guid DESC LIMIT 2;



UPDATE gameobject SET animprogress=100, state=1 WHERE map IN(@NA_MAP, @BE_MAP, @RL_MAP);