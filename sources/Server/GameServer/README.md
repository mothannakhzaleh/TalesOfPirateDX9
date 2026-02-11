GameServer (additional sql statements)
======================================

guild bank
----------
ALTER TABLE gamedb.dbo.guild ADD bank varchar(8000);
alter table gamedb.dbo.guild add gold bigint default(0) NOT NULL;

guild permission
----------------
alter table gamedb.dbo.character drop constraint DF_character_guild_permission;
alter table gamedb.dbo.character alter column guild_permission bigint;
ALTER TABLE gamedb.dbo.character ADD CONSTRAINT DF_character_guild_permission DEFAULT 0 FOR guild_permission;

These queries only needed if there are real guilds. On fresh database is not needed:
------------------------------------------------------------------------------------

For existing (vanilla) guilds do:
---------------------------------
update gamedb.dbo.character set guild_permission = 1 where guild_id > 0 and guild_permission = 0;
update gamedb.dbo.character set guild_permission = 4294967295 where guild_id > 0 and guild_permission = -1;

For guilds that have used guild permissions, but not been updated to the new system (note: untested)
----------------------------------------------------------------------------------------------------
update gamedb.dbo.character
set guild_permission = 4294967295
where guild_id = (select guild_id from gamedb.dbo.guild where leader_id = cha_id) and guild_permission = 255;

guild attr
----------
alter table gamedb.dbo.guild add 
PDEF int default(0) NOT NULL,
MSPD int default(0) NOT NULL,
ASPD int default(0) NOT NULL,
MXATK int default(0) NOT NULL,
DEF  int default(0) NOT NULL,
HIT  int default(0) NOT NULL,
FLEE int default(0) NOT NULL,
HREC int default(0) NOT NULL,
SREC int default(0) NOT NULL,
MXHP int default(0) NOT NULL,
MXSP int default(0) NOT NULL,
level int default(0) NOT NULL;

guild colour
-------------------------------
alter table gamedb.dbo.guild add colour int default(-1) NOT NULL;
alter table gamedb.dbo.guild add icon int default(0) NOT NULL

IGS:

alter table gamedb.dbo.account add IMP int default(0);

Chat colour:
alter table gamedb.dbo.character add chatColour int default(-1) NOT NULL
