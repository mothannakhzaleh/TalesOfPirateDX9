CD AccountServer
START /MIN AccountServer.exe AccountServer.cfg
CD ..
CD GateServer
START /MIN GateServer.exe GateServer.cfg
CD ..
CD GroupServer
START /MIN GroupServer.exe GroupServer.cfg
CD ..
CD GameServer
START /MIN GameServer.exe GameServer00.cfg
::START /MIN GameServer.exe GameServer01.cfg
::START /MIN GameServer.exe GameServer02.cfg
::START /MIN GameServer.exe GameServer03.cfg