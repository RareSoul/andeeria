-- Binding (Voidwalker allinace)
INSERT INTO `gameobject_template` VALUES ('199999','8','465','Stormwind Summoning Circle','','','','35','0','2','0','0','0','0','0','0','83','10','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','');
DELETE FROM spell_script_target WHERE entry = 7728;
INSERT INTO spell_script_target VALUES 
(7728,0,199999);
DELETE FROM gameobject WHERE id =199999;
INSERT INTO `gameobject` VALUES ('400000','199999','0','1','1','-8971.31','1041.68','52.8626','2.83392','0','0','0.988191','0.153228','25','0','1');