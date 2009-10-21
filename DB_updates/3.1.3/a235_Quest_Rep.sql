-- Quest "Wanted: The Heart of Quagmirran" Reputation fix
UPDATE `quest_template` SET 
`RewRepFaction1`='942',
`RewRepValue1`='350',
`RewRepFaction2`='933',
`RewRepValue2`='350',
`RewRepFaction3`='0',
`RewRepValue3`='0',
`RewRepFaction4`='0',
`RewRepValue4`='0',
`RewRepFaction5`='0',
`RewRepValue5`='0'
 WHERE `entry`='11368';