package.path = package.path .. ";" .. lfs.currentdir() .. "\\script\\?.lua"
package.cpath = package.cpath .. ";" .. lfs.currentdir() .. "\\script\\?.dll"

require "common"

--lua ����C++�ӿ�
_g_LuaVM=nil; 		--ȫ��CLuaVM����

_g_operid = 0;
_g_prodname="";
_g_idata = "";
_g_itime = "";

function main(_luaVm) --�ڽű���ʼ����ʱ�����
	_g_LuaVM = LuaVM.ConvertToLuaVM(_luaVm);
	
	_g_prodname = tostring(_g_LuaVM.GetEnvParamString("productname"));	--��ƷӢ������
	_g_operid = msapp.CApp(_g_prodname).GetSetting("oper", 0);
	
	--��ʼ��ʱ��
	_g_idata = msapp.CApp(_g_prodname).GetSetting("idata", tostring(mstime.GetCurrentDateStr(false)));
	_g_itime = msapp.CApp(_g_prodname).GetSetting("itime", tostring(mstime.GetCurrentTimeStr(false)));
	
end	

local function Report()
  local _host = "update.kyan.com.cn";
  local _path = "active";
  local _param = 
    {
      ["apiver"]      = 1;--�ӿڰ汾��
      ["uguid"]       = winapi.GetUserGuid(),
	  ["mainbord"]	  = winapi.GetMainBoardSerialNumber(),
	  ["hdd"]	  	  = winapi.GetDiskSerialNumber(),
	  ["mac"]		  = winapi.GetNetAdapter(),
	  
      ["prod"]        = _g_LuaVM.GetEnvParamString("prouctid"),
	  ["packet"]      = _g_LuaVM.GetEnvParamString("packetid"),
      ["prodver"]     = _g_LuaVM.GetEnvParamString("version"),
	  ["subprod"]     = 3,--�Ӳ�Ʒ
	  ["idate"]		  =  _g_idata,
      ["itime"]		  =  _g_itime,
      ["oper"]        =  _g_operid,
      ["wow64"]       = winapi.IsWindowsX64(),
      ["osver"]       = winapi.GetMicroVersion(),
	  ["vm"]          = booltoint(osinfo.IsVM()),
    };
   
  local _retcode , _context  = HttpGet(_host, 80, _path, _param);
  return _retcode;
end

function DoThread()
	Report();
	
	return 0;
end