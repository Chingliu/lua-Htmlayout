module('ui', package.seeall)

-----------------------------------------------------------------------------
local function search (k, plist)
		for i=1, table.getn(plist) do
        	local v = plist[i][k]     -- try `i'-th superclass
	        if v then return v end
		end
end

--����һ�����ؼ̳е����࣬��������ȼ��ɲ���˳�����
function createClass (...)
      local c = {}        -- new class

      -- class will search for each method in the list of its
      -- parents (`arg' is the list of parents)
      setmetatable(c, 
      {	__index = function (t, k)
		local v =  search(k, arg)
		return v
	end})

      -- prepare `c' to be the metatable of its instances
      c.__index = c;
      function c:init()
      end

      function c:clear_all_parent()
		for k, v in ipairs(arg) do
			rawset(v, 'obj', nil);
			v = nil;
		end
		collectgarbage();
      end

      -- define a new constructor for this new class
      function c:new (o)
        o = o or {}
        setmetatable(o, c)
		o:init();
        return o
      end

      -- return new class
      return c
end

--��ȫ���ؿ⣬ʧ��ʱ���жϴ���
function saferequire(libname)
	local x = function()
		require(libname);
	end
	return pcall(x);
end