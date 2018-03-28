
local searchPath = tostring(winapi.GetModuleFilePath(winapi.GetModuleHandle("lua-5.1.4.dll")));

package.path = package.path .. ";" .. searchPath .. "\\script\\?.lua"
package.cpath = package.cpath .. ";" .. searchPath .. "\\script\\?.dll"

require "common"


_g_silent=false;		--�Ƿ�Ĭ��װ
_g_operid = 1;
_g_key = "defaultkey";
_g_prodname = "";
_g_idata = "";	--����İ�װ����
_g_itime = "";	--����İ�װʱ��

--��ȡ�ļ�����
local function GetFileName(fullPath)
	dest_filename = nil;
	fn_flag = string.find(fullPath, "\\");
	if fn_flag then
		dest_filename = string.match(fullPath, ".+\\([^\\]*%.%w+)$");
	end
	
	fn_flag = string.find(fullPath, "/");
	if fn_flag then
		dest_filename = string.match(fullPath, ".+/([^/]*%.%w+)$");
	end
	return dest_filename;
end

--��ȡ������չ�����ļ�����
local function GetFileNameNotExtension(fileName)
	local idx = fileName:match(".+()%.%w+$")  
		if(idx) then  
			return fileName:sub(1, idx-1)  
		else  
			return fileName  
		end  
	return nil;
end



-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

--��ȡ��ʱ�ļ����·��
local function GetTempCheckOutPath()
	local path = _g_LuaVM.LuaCallFunction("Setup", "GetTempCheckOutPath", "");
	local json = cjson.decode(tostring(path));
	return tostring(json["path"]);
end



local function GetObject(_name, _func)
	if(_g_LuaVM == nil) then return nil end;
	return _func(_g_LuaVM.GetObject(_name));
end


local function modifyXmlConfig()

	local strFullPath = GetTempCheckOutPath();
	local infdir = string.format("%s\\*.xml", strFullPath);
	local FileFind = winfunc.WinFileFind();
	local bFindFile = FileFind.FindFile(infdir, 0);
	while bFindFile == true do
		bFindFile = FileFind.FindNextFile();
		local inffile = tostring(FileFind.GetFilePath());
		if string.find(inffile, "setup.xml")  == nil then
			doc = rapidxml.open(inffile);
			if  doc then
				local root = doc:root();
				--root:setattr("encoding","UTF-8");
				local mscomcfg = root:select("mscomcfg")[1];
				local envinfo = mscomcfg:select("envinfo");
				if table.getn(envinfo) and envinfo[1] ~= nil then
					oper = envinfo[1]:select("oper");
					if table.getn(oper) and oper[1] ~= nil then
						oper[1]:settext(GetOper());
						doc:saveutf8(inffile,true);
					end
				end
			end
		end
		
	end
end

--lua ����C++�ӿ�

 
 
--local json = require  "cjson"

function main(_luaVm) --�ڽű���ʼ����ʱ�����
	InitLuaVM(_luaVm);
	
	print("---------------------------------------setup---------------------------------------");
	_g_silent = inttobool(_g_LuaVM.GetEnvParam("silent"));
	_g_prodname = tostring(_g_LuaVM.GetEnvParamString("productname"));	--��ƷӢ������
	
	_g_operid = msapp.CApp(_g_prodname).GetSetting("oper", _g_LuaVM.GetEnvParam("new_operid"));
	
	if _g_operid == 0 then
		_g_operid = _g_LuaVM.GetEnvParam("oper");
	end
	
	print( "silent-->"..tostring(_g_silent) .."		operid---->".. tostring(_g_operid));
	
	_g_idata = msapp.CApp(_g_prodname).GetSetting("idata", tostring(mstime.GetCurrentDateStr(false)));
	_g_itime = msapp.CApp(_g_prodname).GetSetting("itime", tostring(mstime.GetCurrentTimeStr(false)));
	
	print( "idata : " .. tostring(_g_idata) .. "	itime:" .. tostring(_g_itime) );
end



--���ð�װ·��
local function SetProgramPosition( path )
	local json =  {};
	json["path"] = path;
	return _g_LuaVM.LuaCallFunction("Setup", "SetProgramPosition", cjson.encode(json));
end


--��ȡ��װ·��
local function GetProgramPosition()
	local path = _g_LuaVM.LuaCallFunction("Setup", "GetProgramPosition", "");
	local json = cjson.decode(tostring(path));
	return tostring(json["path"]);
end





--��ȡ�ؼ�����ʾ״̬
local function GetControlCheckState( ctrlName )
	local json = {};
	json["ctrl"] = tostring(ctrlName);
	local result = _g_LuaVM.LuaCallFunction("Setup", "GetControlCheckState", cjson.encode(json));
	json  = cjson.decode(tostring(result));
	return json["state"];
end


--�ж��Ƿ�װ��
 function IsProductExists()
	local reg = string.format("%s\\%s", "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall", tostring(_g_LuaVM.GetEnvParamString("productname")));
	local regObj = winfunc.WinRegKey();
	local bRet = regObj.Open("HKEY_LOCAL_MACHINE", reg, "KEY_READ_WRITE");
	if not bRet then 
		return false; 
	end
	
	return true;
 end
 
 --��ȡ��ǰ�汾�İ�װ·��
 function GetOldProgramPosition()
	local reg = string.format("%s\\%s", "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall", tostring(_g_LuaVM.GetEnvParamString("productname")));
	local regObj = winfunc.WinRegKey();
	regObj.Open("HKEY_LOCAL_MACHINE", reg, "KEY_READ_WRITE");
	local pos = regObj.QueryStringValue("InstallLocation");
	return tostring(pos);
 end

 
 --��ȡж��·��
 function GetUninstallPosition()
	local reg = string.format("%s\\%s", "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall", tostring(_g_LuaVM.GetEnvParamString("productname")));
	local regObj = winfunc.WinRegKey();
	regObj.Open("HKEY_LOCAL_MACHINE", reg, "KEY_READ_WRITE");
	local pos = regObj.QueryStringValue("UninstallString");
	return tostring(pos);
 end
 
 
 --���ð�װ����
 function SetProgress( n )
	local json =  {};
	json["pos"] = tonumber( n );
	return _g_LuaVM.LuaCallFunction("Setup", "SetProgress", cjson.encode(json));
 end
 
 

 
 
--��װ�����ʼ��
function LuaInitSetup( _param )
	--��ȡ��ǰ�汾�İ�װ·��
	local path = "";
	if winapi.IsWindowsX64() == true then
		path = tostring(windir.PathProviderWin(windir.DIR_PROGRAM_FILESX86));
	else
		path = tostring(windir.PathProviderWin(windir.DIR_PROGRAM_FILES));
	end
	
	if IsProductExists() then
		path = GetOldProgramPosition();
	end
	
	local prod = tostring(_g_LuaVM.GetEnvParamString("productname"));
	--ƥ�俴�ǲ��ǰ�װ�ڱ��Ŀ¼����
	
	local i, _ = string.find(path, prod);
	if i == nil then
		path = string.gsub(string.format("%s\\%s", path, prod), "\\\\", "\\");
	end
	
	--���ð�װ·��
	SetProgramPosition(path);
	local json = {};
	json["result"] = true;
	return cjson.encode(json);
end

function ProgramPositionChange( _param )
	
	local json = {};
	local param = cjson.decode(_param);
	local path = tostring(param["path"]);
	local prod = tostring(_g_LuaVM.GetEnvParamString("productname"));
	--ƥ�俴�ǲ��ǰ�װ�ڱ��Ŀ¼����
	local i, _ = string.find(path, prod)
	if i == nil then
		path = string.gsub(string.format("%s\\%s", path, prod), "\\\\", "\\");
		SetProgramPosition(path);
	end
	
	json["result"] = true;
	return cjson.encode(json);
end



function CopyFiles()
	local pos = GetProgramPosition();
	
	print(pos);
	local cur = GetTempCheckOutPath();
	print(cur);
	
	CopyFile(cur.."\\common file\\mscom\\comebase.dll"		, pos.."\\common file\\mscom\\comebase.dll");
	CopyFile(cur.."\\common file\\mscom\\envmgr.dll",  pos.."\\common file\\mscom\\envmgr.dll");
	CopyFile(cur.."\\common file\\mscom\\filemonitor.dll" , pos.."\\common file\\mscom\\filemonitor.dll");
	CopyFile(cur.."\\common file\\mscom\\msgbus.dll" , pos.."\\common file\\mscom\\msgbus.dll");
	CopyFile(cur.."\\common file\\mscom\\msxml3w.dll" , pos.."\\common file\\mscom\\msxml3w.dll");
	CopyFile(cur.."\\common file\\mscom\\npc32.dll",  pos.."\\common file\\mscom\\npc32.dll");
	CopyFile(cur.."\\common file\\mscom\\npc64.dll",  pos.."\\common file\\mscom\\npc64.dll");
	CopyFile(cur.."\\common file\\mscom\\traywnd.dll", pos.."\\common file\\mscom\\traywnd.dll");
	CopyFile(cur.."\\common file\\mscom\\luahost.dll", pos.."\\common file\\mscom\\luahost.dll");
	CopyFile(cur.."\\common file\\mscom\\managedb.dll", pos.."\\common file\\mscom\\managedb.dll");
	CopyFile(cur.."\\common file\\mscom\\sqlitedb.dll", pos.."\\common file\\mscom\\sqlitedb.dll");
	CopyFile(cur.."\\common file\\mscom\\sqlitesdb.dll", pos.."\\common file\\mscom\\sqlitesdb.dll");
	CopyFile(cur.."\\common file\\mscom\\downloadmgr.dll", pos.."\\common file\\mscom\\downloadmgr.dll");
	CopyFile(cur.."\\common file\\mscom\\DeviceControl.dll", pos.."\\common file\\mscom\\DeviceControl.dll");
	CopyFile(cur.."\\common file\\mscom\\msgcrt.dll", pos.."\\common file\\mscom\\msgcrt.dll");
	CopyFile(cur.."\\common file\\mscom\\signalapp.dll", pos.."\\common file\\mscom\\signalapp.dll");
	CopyFile(cur.."\\common file\\mscom\\softeng.dll", pos.."\\common file\\mscom\\softeng.dll");
	
	CopyFile(cur.."\\imgdecoder-gdip.dll", pos.."\\imgdecoder-gdip.dll");
	CopyFile(cur.."\\imgdecoder-png.dll", pos.."\\imgdecoder-png.dll");
	CopyFile(cur.."\\imgdecoder-stb.dll", pos.."\\imgdecoder-stb.dll");
	CopyFile(cur.."\\imgdecoder-wic.dll", pos.."\\imgdecoder-wic.dll");
	CopyFile(cur.."\\render-gdi.dll", pos.."\\render-gdi.dll");
	--CopyFile(cur.."\\render-skia.dll", pos.."\\render-skia.dll");
	CopyFile(cur.."\\resprovider-zip.dll", pos.."\\resprovider-zip.dll");
	--CopyFile(cur.."\\scriptmodule-lua.dll",pos.."\\scriptmodule-lua.dll");
	--CopyFile(cur.."\\soui.dll",pos.."\\soui.dll");
	--CopyFile(cur.."\\soui-sys-resource.dll",pos.."\\soui-sys-resource.dll");
	CopyFile(cur.."\\translator.dll",pos.."\\translator.dll");
	CopyFile(cur.."\\utilities.dll",pos.."\\utilities.dll");
	CopyFile(cur.."\\soui.dll",pos.."\\soui.dll");
	
	--CopyFile(cur.."\\DuiLib.dll", pos.."\\DuiLib.dll");
	CopyFile(cur.."\\FreeImage.dll",pos.."\\FreeImage.dll");
	CopyFile(cur.."\\SciLexer.dll",pos.."\\SciLexer.dll");
	
	CopyFile(cur.."\\syslay.dll", pos.."\\syslay.dll");
	CopyFile(cur.."\\stdlog.dll", pos.."\\stdlog.dll");
	CopyFile(cur.."\\msvcp100.dll", pos.."\\msvcp100.dll");
	CopyFile(cur.."\\msvcr100.dll", pos.."\\msvcr100.dll");
	CopyFile(cur.."\\lua-5.1.4.dll", pos.."\\lua-5.1.4.dll");
	CopyFile(cur.."\\mscomcfg.xml",pos.."\\mscomcfg.xml");
	
	CopyFile(cur.."\\uninstall.exe", pos.."\\uninstall.exe");
	CopyFile(cur.."\\uninstall.xml", pos.."\\uninstall.xml");
	
	
	CopyFile(cur.."\\x86\\ShellExt32.dll", pos.."\\x86\\ShellExt32.dll");
	CopyFile(cur.."\\x64\\ShellExt64.dll", pos.."\\x64\\ShellExt64.dll");

	CopyFile(cur.."\\script\\uninst.lua",pos.."\\script\\uninst.lua");
	CopyFile(cur.."\\script\\common.lua",pos.."\\script\\common.lua");
	CopyFile(cur.."\\script\\JCDesk.lua",pos.."\\script\\JCDesk.lua");
	CopyFile(cur.."\\script\\setup.lua",pos.."\\script\\setup.lua");
	CopyFile(cur.."\\script\\JCSrv.lua",pos.."\\script\\JCSrv.lua");
	
	CopyFile(cur.."\\common file\\jcfs\\fSearch.dll",pos.."\\common file\\jcfs\\fSearch.dll");
	CopyFile(cur.."\\common file\\jcfs\\JCfsClient.dll",pos.."\\common file\\jcfs\\JCfsClient.dll");
	CopyFile(cur.."\\common file\\jcfs\\JCfsUI.dll",pos.."\\common file\\jcfs\\JCfsUI.dll");
	
	CopyFile(cur.."\\common file\\tray\\DeskTray.dll",pos.."\\common file\\tray\\DeskTray.dll");
	CopyFile(cur.."\\common file\\update\\UpdateUI.dll",pos.."\\common file\\update\\UpdateUI.dll");
	

	CopyFile(cur.."\\JCSrv.exe",pos.."\\JCSrv.exe");
	CopyFile(cur.."\\JCSrv.mscom",pos.."\\JCSrv.mscom");
	CopyFile(cur.."\\JCSrv.xml",pos.."\\JCSrv.xml");
	
	
	CopyFile(cur.."\\soft.db",pos.."\\soft.db");
	
	CopyFile(cur.."\\JCTray.exe",pos.."\\JCTray.exe");
	CopyFile(cur.."\\JCTray.mscom",pos.."\\JCTray.mscom");
	CopyFile(cur.."\\JCTray.xml",pos.."\\JCTray.xml");
	
	CopyFile(cur.."\\JCfs.exe",pos.."\\JCfs.exe");
	CopyFile(cur.."\\JCfs.mscom",pos.."\\JCfs.mscom");
	CopyFile(cur.."\\JCfs.xml",pos.."\\JCfs.xml");
	
	
	CopyFile(cur.."\\update.exe", pos.."\\update.exe");
	CopyFile(cur.."\\update.mscom",pos.."\\update.mscom");
	CopyFile(cur.."\\update.xml", pos.."\\update.xml");
	
	CopyFile(cur.."\\JCmd.exe", pos.."\\JCmd.exe");
	
	--copy license
	CopyFile(cur.."\\directui license.txt",pos.."\\directui license.txt");
	CopyFile(cur.."\\duilib license.txt",pos.."\\duilib license.txt");
	CopyFile(cur.."\\soui license.txt",pos.."\\soui license.txt");
	return true;
end


--������ݷ�ʽ
local function CreateShortCut()

	print("��ʼ������ݷ�ʽ");
	local prodName = tostring(_g_LuaVM.GetEnvParamString("friendprodname"));
	local cur = GetProgramPosition();
	local TargetPath = string.format("%s\\%s.exe", cur,tostring("JCfs"));
  
  --��ӵ������ݷ�ʽ
	if GetControlCheckState("addto_disktop_checkbox") then
		print("��ӵ������ݷ�ʽ");
		local DesktopPos = tostring(quicklink.GetDesktopPos());
		local LinkPath = string.format("%s\\%s.lnk", DesktopPos, prodName);
		quicklink.CreateLink(LinkPath, TargetPath, "", "", "");
	end
  
  
	--�����������ݷ�ʽ
	if GetControlCheckState("addto_taskbar_checkbox") then
		print("������ʼ�˵��������");
		local QuickLaunchPos = tostring(quicklink.GetQuickLaunchPos());
		local LinkPath = string.format("%s\\%s.lnk", QuickLaunchPos, prodName);
		quicklink.CreateTaskBar(LinkPath, TargetPath, "", "", "");
   
		--����������
		print("����������������ݷ�ʽ");
		local StartMenuPos = tostring(quicklink.GetStartMenuPos());
		LinkPath = string.format("%s\\%s\\���� %s.lnk", StartMenuPos, prodName, prodName);
		quicklink.CreateLink(LinkPath, TargetPath, "", "", "");
		
		
		print("����������ж�ؿ�ݷ�ʽ");
		LinkPath = string.format("%s\\%s\\ж�� %s.lnk", StartMenuPos, prodName, prodName);
		quicklink.CreateLink(LinkPath, string.format("%s\\uninstall.exe", cur), "", "", "");
	
		if winapi.GetMicroVersion()<14 then
			print("������������ݷ�ʽ");
			quicklink.CreateTaskBar(string.format("\\%s.lnk", prodName), TargetPath, "", "", "");
		end
  end
  
   return true;
end


--����ж��ע���
local function CreateUninstallReg()
  local reg = string.format("%s\\%s", "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall", tostring(_g_LuaVM.GetEnvParamString("productname")));
  local regObj = winfunc.WinRegKey();
  local bRet = regObj.Create("HKEY_LOCAL_MACHINE", reg , "REG_NONE",0, "KEY_READ_WRITE");
  if not bRet then
    return false;
  end
  
  local prodName = tostring(_g_LuaVM.GetEnvParamString("friendprodname"));
  local cur = GetProgramPosition();
  
  regObj.SetStringValue("DisplayName", prodName, "REG_SZ");
  regObj.SetStringValue("DisplayVersion", tostring(_g_LuaVM.GetEnvParamString("version")), "REG_SZ");
  regObj.SetStringValue("InstallLocation", cur, "REG_SZ");
  UninstallPath = string.format("%s\\uninstall.exe", cur);
  regObj.SetStringValue("UninstallString", UninstallPath, "REG_SZ");
  
  local ExePath = string.format("%s\\%s.exe", cur, tostring(_g_LuaVM.GetEnvParamString("productname")));
  regObj.SetStringValue("DisplayIcon", ExePath, "REG_SZ");
  return true;
end

local function DoUninstall()
	local tempExe = string.format("%s\\%s\\%s", GetTempPath(), tostring(mstime.GetCurrentDateStr(false)), "uninstall.exe");
	local exePos = GetUninstallPosition();
	CopyFile(exePos, tempExe);
	local cmd = string.format("\"%s\" p=%s|s=1", tempExe, GetOldProgramPosition());
	print(cmd);
	winapi.Execute(tempExe, cmd , true, false, -1);
	return true;
end


--��ȡ��װ����
local function GetInstallType()
	return 1;
end



local function Report()
  local _host = "update.kyan.com.cn";
  local _path = "install";
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
	  ["idate"]		  =  _g_idata,
      ["itime"]		  =  _g_itime,
      ["oper"]        = _g_operid,
      ["wow64"]       = winapi.IsWindowsX64(),
      ["osver"]          = winapi.GetMicroVersion(),
	  ["vm"]          = booltoint(osinfo.IsVM()),
	  ["type"]		  = GetInstallType(),
    };
   
  local _retcode , _context  = HttpGet(_host, 80, _path, _param);
  return _retcode;
end


--ִ�а�װ�ű�
function Do( param )


	local result = {};
	result["result"] = 0;
	SetProgress(50);
	
	local bInst = IsProductExists();
	if bInst then
	 DoUninstall();
	end
	
	
	
	SetProgress(55);
	
	repeat
		
		
		if WinService.IsExistedService("JCSrv") then
			print( "������ڣ�ִ�з���ж�ؽű���" );
			local Ret = winapi.ShellExecute("", GetProgramPosition().."\\JCSrv.exe", "/UnregServer" ,nil, 0,1);
			if not Ret then
				print( "ִ��ж�ط���ű�ʧ�ܡ�" );
				break
			end
		end
	

		SetProgress(60);
		
		if not CopyFiles() then
			print("�����ļ�ʧ��");
			break;
		end
		
		SetProgress(70);
		
		
		SetProgress(80);
		
		if not CreateUninstallReg() then
		  print("����ж��ע���ʧ��");
		  break;
		end
		
		SetProgress(90);
		
		--���ð汾��
		msapp.CApp((_g_prodname)).SetVersion(tostring(_g_LuaVM.GetEnvParamString("version")));
		
		--���ò�ƷID
		msapp.CApp((_g_prodname)).SetSetting("prod", tostring(_g_LuaVM.GetEnvParamString("prouctid")));
		
		--���ð�ID
		msapp.CApp((_g_prodname)).SetSetting("packet", tostring(_g_LuaVM.GetEnvParamString("packetid")));
		
		--�޸�����ID
		msapp.CApp(_g_prodname).SetSetting("oper", _g_operid);
		
		--���ð�װ·��
		msapp.CApp(_g_prodname).SetSetupPath(GetProgramPosition());
		
		--���ð�װʱ��
		msapp.CApp((_g_prodname)).SetSetting("itime", tostring(_g_itime));
		
		--���ð�װ����
		msapp.CApp((_g_prodname)).SetSetting("idate", tostring(_g_idata));
		
		--����Ӳ��ID
		msapp.GetDiskSerial();
		
		
		--���°�װʱ��
		msapp.CApp((_g_prodname)).SetSetting("idate", tostring(_g_idata) );
		msapp.CApp((_g_prodname)).SetSetting("itime", tostring(_g_itime) );
		--]]
		
		print("��ʼ���Shell���");
		--ע��shell���
		
		
		
		winapi.DllRegisterServer(tostring(GetProgramPosition().."\\x86\\ShellExt32.dll"), true);
		winapi.DllRegisterServer(tostring(GetProgramPosition().."\\x64\\ShellExt64.dll"), true);
		--ע��RPC�ͻ������
		local Ret = winapi.DllRegisterServer(GetProgramPosition().."\\common file\\jcfs\\JCfsClient.dll", true);
		if not Ret then
			print("ע��Rpc����ʧ��");
			break;
		end
		
		
		print("��ʼע�����");
		local Ret = winapi.ShellExecute("open", GetProgramPosition().."\\JCSrv.exe", "/Service" ,"", 0, 1);
		if not Ret then
			print("��������ʧ��");
			break;
		end
		
		
		
		if not WinService.IsExistedService("JCSrv") then
			print("��Ȼע�����ɽ���ִ�гɹ�������δ��⵽������ڡ�");
			break
		end
		
		for v = 1 , 3 , 1 do --�ظ��������������������ʧ��ֱ���˳�
			if not WinService.IsSrvRunning("JCSrv") then
				print("������δ��������Ҫ��������");
				if not WinService.StartService("JCSrv", "") then
					print("��������ʧ��");
				else
					print("��������ɹ�");
					break;
				end
			end
			
			winapi.Sleep(1000);
		end
		
		
		if not WinService.IsSrvRunning("JCSrv") then
			print("ֱ�����û�м�⵽�������У���װʧ�ܡ�")
			break;
		end
		
		--����ٴ�����ݷ�ʽ
		if not CreateShortCut() then
			print("������ݷ�ʽʧ��");
		  break;
		end
		
	
		Report();
		SetProgress(100);
		
		result["result"] = 1; --��װ�ɹ�
	until true;
	return cjson.encode(result);
end





