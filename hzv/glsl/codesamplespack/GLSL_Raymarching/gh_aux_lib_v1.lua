--[[
GLSL Hacker Auxiliary Library. 

Homepage		: www.glslhacker.com

ChangeLog:
-------------
2012-09-19
	- Intial version [JeGX - jegx@geeks3d.com]
	
--]]









--[[ 
Core of LUA Object Instancing 
 class.lua: see http://lua-users.org/wiki/SimpleLuaClasses 
 Compatible with Lua 5.1 (not 5.0).
 --]]
function class(base,ctor)
  local c = {}     -- a new class instance
  if not ctor and type(base) == 'function' then
      ctor = base
      base = nil
  elseif type(base) == 'table' then
   -- our new class is a shallow copy of the base class!
      for i,v in pairs(base) do
          c[i] = v
      end
      c._base = base
  end
  -- the class will be the metatable for all its objects,
  -- and they will look up their methods in it.
  c.__index = c

  -- expose a ctor which can be called by <classname>(<args>)
  local mt = {}
  mt.__call = function(class_tbl,...)
    local obj = {}
    setmetatable(obj,c)
    if ctor then
       ctor(obj,...)
    else
    -- make sure that any stuff from the base class is initialized!
       if base and base.init then
         base.init(obj,...)
       end
    end
    return obj
  end
  c.init = ctor
  c.is_a = function(self,klass)
      local m = getmetatable(self)
      while m do
         if m == klass then return true end
         m = m._base
      end
      return false
    end
  setmetatable(c,mt)
  return c
end


--[[
=======================================================================
================  MATH CORE   functions  ==============================
=======================================================================
--]]

--[[
Math constants
--]]
GH_PI = math.pi
GH_EPSILON = 0.000001
GH_PI_OVER_180 = math.pi/180
GH_180_OVER_PI = 180/math.pi

function GH_InitRandom()
	math.randomseed(os.time()) -- seed and pop
end

function GH_random(a, b)
	if (a > b) then
		local c = b
		b = a
		a = c
	end
	local delta = b-a
	return (a + math.random()*delta)
end

function GH_Rad2Deg( radian )
	return( radian * GH_180_OVER_PI )
end

function GH_Deg2Rad( degree )
	return( degree * GH_PI_OVER_180 )
end

function GH_Lerp( a, b, t)
	return (a + (b - a)*t)
end

function GH_Max(a, b)
	if (a > b) then
		return a;
	end
	return b;
end

function GH_Min(a, b)
	if (a < b) then
		return a;
	end
	return b;
end

function GH_Clamp( n, _min, _max)
	if (n > _max) then 
		n = _max; 
	end
	if (n < _min) then 
		n = _min; 
	end
	return n;
end

function GH_Interpolate( actual, goal, speed )
	local delta = goal - actual
	if( math.abs(delta)<GH_EPSILON) then
		return goal
	end
	return (actual + delta * GH_Min(speed, 1.0))
end



--[[
=======================================================================
================  VECTORS  functions  =================================
=======================================================================
--]]

--[[
Vector creation.
myVec = vec3();
--]]
vec3 = class(function(vec,x,y,z)
	if not z then z=0 end
	if not y then y=0 end
	if not x then x=0 end
    vec:set(x,y,z)
 end)

--[[
Vector accessor - Get/Set
--]]
function vec3.set(vec, x, y, z)
	if type(x) == 'table' and getmetatable(x) == vec3 then
		local po = x;
		x = po.x;
		y = po.y;
		z = po.z;
	end
	vec.x = x;
	vec.y = y;
	vec.z = z;
end

function vec3.get(v)
  return v.x, v.y, v.z;
end
function vec3.get_x(v)
  return v.x;
end
function vec3.get_y(v)
  return v.y;
end
function vec3.get_z(v)
  return v.z;
end

--[[
Vector equality.
v1 == v2 ?
--]]
function vec3.__eq(v1, v2)
  return (v1.x==v2.x) and (v1.y==v2.y) and (v1.z==v2.z);
end

function VEC3_IsEqual(v1, v2)
  return v1==v2;
end

--[[
Vector addition.
vec3 addition is '+','-'
--]]
function vec3.__add(v1, v2)
  return vec3(v1.x+v2.x, v1.y+v2.y, v1.z+v2.z)
end

function vec3.__sub(v1, v2)
  return vec3(v1.x-v2.x, v1.y-v2.y, v1.z-v2.z)
end

function vec3.add(v1, v2)
  return v1+v2;
end

function vec3.sub(v1, v2)
  return v1-v2;
end

--[[
unitary minus  (e.g in the expression f(-p))
Negate.
--]]
function vec3.__unm(v)
  return vec3(-v.x, -v.y, -v.z)
end

function vec3.neg(v)
	v.x = -v.x;
	v.y = -v.y;
	v.z = -v.z;
end

--[[
scalar and component-wise multiplication is '*'
--]]
function vec3.__mul(v, s)
	if type(s) == 'table' then
		return vec3( s.x*v.x, s.y*v.y, s.z*v.z );
	else
		return vec3( s*v.x, s*v.y, s*v.z );
	end
end

function vec3.mul(v, s)
	if type(s) == 'table' then
		v.x = s.x*v.x;
		v.y = s.y*v.y;
		v.z = s.z*v.z;
	else
		v.x = s*v.x;
		v.y = s*v.y;
		v.z = s*v.z;
	end
end

--[[
DOT product.
--]]
function vec3.dot(v1, v2)
   return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z
end

--[[
CROSS product.
--]]
function vec3.cross(v1, v2)
	return vec3(v1.y*v2.z - v1.z*v2.y,
	            v1.z*v2.x - v1.x*v2.z,
			    v1.x*v2.y - v1.y*v2.x)
end

function vec3.cross_v2(v1, v2)
	local x = v1.y*v2.z - v1.z*v2.y
	local y = v1.z*v2.x - v1.x*v2.z
	local z = v1.x*v2.y - v1.y*v2.x
	v1.x = x
	v1.y = y
	v1.z = z
end

--[[
Vector norm or length or magnitude.
--]]
function vec3.square_len(v)
	return v.x*v.x + v.y*v.y + v.z*v.z
end

function vec3.len(v)
	return math.sqrt(v:square_len())
end

--[[
Vector normalization.
--]]
function vec3.normalize(v)
	local norm = v:len();
	local multiplier;
	if( norm > 0 ) then
		multiplier = 1.0/norm;
	else
		multiplier = 0.00001;
	end
		
	v.x = v.x * multiplier;
	v.y = v.y * multiplier;
	v.z = v.z * multiplier;
end

--[[
Is null vector ?
--]]
function vec3.is_null(v)
	return (v.x==0 and v.y==0 and v.z==0);
end

--[[
Distance between vectors
--]]
function vec3.square_distance(v1, v2)
	local dx = v1.x-v2.x;
	local dy = v1.y-v2.y;
	local dz = v1.z-v2.z;
	return dx*dx + dy*dy + dz*dz;
end

function vec3:distance(v1, v2)
	return math.sqrt(v1:square_distance(v2));
end

--[[
Linear Interpolation
--]]
function vec3.lerp(a, b, t)
	return vec3( a.x + (b.x - a.x)*t, 
	             a.y + (b.y - a.y)*t, 
				 a.z + (b.z - a.z)*t );
end

function VEC3_Lerp(a, b, t)
	return vec3( a.x + (b.x - a.x)*t, 
	             a.y + (b.y - a.y)*t, 
				 a.z + (b.z - a.z)*t );
end

function vec3.to_string(v)	
	return string.format('%f,%f,%f', v.x, v.y, v.z);
end

function vec3.dump(v, name) 
	gh_utils.trace(ID, string.format("%s = (%f %f %f)",name, v.x, v.y, v.z))
end



--[[
=======================================================================
============  GLSL Hacker OBJECTS  functions  =========================
=======================================================================
--]]

--[[
axis is a vec3.
dist is a real.
--]]
function GH_MoveObjectAlongAxis(object, dist, axis)
	local v = vec3();
	v.x, v.y, v.z = gh_object.get_position(object)
	v = v + (axis*dist)
	gh_object.set_position(object, v.x, v.y, v.z)
end

--[[
Returns a vec3
--]]
function GH_GetObjectPosition(object)
	local v = vec3();
	v.x, v.y, v.z = gh_object.get_position(object)
	return v
end

--[[
p is a vec3
--]]
function GH_SetObjectPosition(object, p)
	gh_object.set_position(object, p.x, p.y, p.z);
end


--[[
t varies between 0.0 and 1.0.
Returns a vec3
--]]
function GH_InterpolateObjectPosition(object1, object2, t)
	local p1 = GH_GetObjectPosition(object1)
	local p2 = GH_GetObjectPosition(object2)
	return vec3(p1.x + (p2.x-p1.x)*t,
	            p1.y + (p2.y-p1.y)*t,
	            p1.z + (p2.z-p1.z)*t)
end




--[[
=======================================================================
============  GLSL Hacker CAMERA  functions  ==========================
=======================================================================
--]]

_do_rotate = 0
_camera_old_mouse_x = 0
_camera_old_mouse_y = 0
_orbit_yaw = 0
_orbit_pitch = 0


function _init_camera_orbit(camera, x, y)
	_camera_old_mouse_x = x
	_camera_old_mouse_y = y
end

function _init_camera_angles(camera, pitch, yaw)
	_orbit_yaw = yaw
	_orbit_pitch = pitch
end


function _rotate_camera_position_around_point(camera, lookat_point_x, lookat_point_y, lookat_point_z, pitch, yaw, roll)
	local cam_pos_x, cam_pos_y, cam_pos_z = gh_object.get_position(camera)
		
	local vx = cam_pos_x - lookat_point_x
	local vy = cam_pos_y - lookat_point_y
	local vz = cam_pos_z - lookat_point_z

	local angX = pitch * GH_PI_OVER_180
	local angY = yaw * GH_PI_OVER_180
	local angZ = roll * GH_PI_OVER_180

	local mag = math.sqrt(vx*vx + vy*vy + vz*vz)

	local new_cam_pos_x = lookat_point_x + mag * math.cos(angY) * math.cos(angX)
	local new_cam_pos_y = lookat_point_y + mag * math.sin(angX)
	local new_cam_pos_z = lookat_point_z + mag * math.sin(angY) * math.cos(angX)

	gh_camera.set_position(camera, new_cam_pos_x, new_cam_pos_y, new_cam_pos_z)
end

function _rotate_camera_orbit(camera, x, y, lookat_x, lookat_y, lookat_z)
	local dx = x - _camera_old_mouse_x
	local dy = y - _camera_old_mouse_y

	_camera_old_mouse_x = x
	_camera_old_mouse_y = y

	local dyaw = dx * 0.1
	local dpitch = dy * 0.1

	_orbit_yaw = _orbit_yaw + dyaw
	_orbit_pitch = _orbit_pitch + dpitch
	
	if (_orbit_pitch >= 80.0) then
		_orbit_pitch = 80.0
	end
	
	if (_orbit_pitch <= -80.0) then
		_orbit_pitch = -80.0
	end

	_rotate_camera_position_around_point(camera, lookat_x, lookat_y, lookat_z, _orbit_pitch, _orbit_yaw, 0.0)
end

function GH_DoCameraOrbit(camera, lookat_x, lookat_y, lookat_z)
	local LEFT_BUTTON = 1
	local is_down = gh_input.mouse_get_button_state(LEFT_BUTTON) 
	if ((is_down == 1) and (_do_rotate == 0)) then
		local mx
		local my
		mx, my = gh_input.mouse_getpos()
		_init_camera_orbit(camera, mx, my)
		_do_rotate = 1
	end
	
	if (is_down == 0) then
		_do_rotate = 0
	end
	
	if (_do_rotate == 1) then
		local mx
		local my
		mx, my = gh_input.mouse_getpos()
		
		if ((mx ~= _camera_old_mouse_x) or (my ~= _camera_old_mouse_y)) then
			--_rotate_camera_orbit(camera, mx, my, lookat_x, lookat_y, lookat_z)
			
			local cam_lookat = vec3()
			cam_lookat:set(lookat_x, lookat_y, lookat_z)
			_rotate_camera_orbit_v2(camera, mx, my, cam_lookat)
			
		end
	end
end





function _rotate_camera_position_around_point_v2(camera, lookat_point, pitch, yaw, roll)
	local cam_pos = vec3()
	cam_pos.x, cam_pos.y, cam_pos.z = gh_object.get_position(camera)
		
	local v = vec3()
	v = cam_pos - lookat_point
	
	local angX = pitch * GH_PI_OVER_180
	local angY = yaw * GH_PI_OVER_180
	local angZ = roll * GH_PI_OVER_180

	local mag = v:len()

	local new_cam_pos_x = lookat_point.x + mag * math.cos(angY) * math.cos(angX)
	local new_cam_pos_y = lookat_point.y + mag * math.sin(angX)
	local new_cam_pos_z = lookat_point.z + mag * math.sin(angY) * math.cos(angX)

	gh_camera.set_position(camera, new_cam_pos_x, new_cam_pos_y, new_cam_pos_z)
end

function _rotate_camera_orbit_v2(camera, x, y, cam_lookat)
	local dx = x - _camera_old_mouse_x
	local dy = y - _camera_old_mouse_y

	_camera_old_mouse_x = x
	_camera_old_mouse_y = y

	local dyaw = dx * 0.1
	local dpitch = dy * 0.1

	_orbit_yaw = _orbit_yaw + dyaw
	_orbit_pitch = _orbit_pitch + dpitch
	
	if (_orbit_pitch >= 80.0) then
		_orbit_pitch = 80.0
	end
	
	if (_orbit_pitch <= -80.0) then
		_orbit_pitch = -80.0
	end

	_rotate_camera_position_around_point_v2(camera, cam_lookat, _orbit_pitch, _orbit_yaw, 0.0)
end

function GH_DoCameraOrbit_v2(camera, cam_lookat)
	local LEFT_BUTTON = 1
	local is_down = gh_input.mouse_get_button_state(LEFT_BUTTON) 
	if ((is_down == 1) and (_do_rotate == 0)) then
		local mx
		local my
		mx, my = gh_input.mouse_getpos()
		_init_camera_orbit(camera, mx, my)
		_do_rotate = 1
	end
	
	if (is_down == 0) then
		_do_rotate = 0
	end
	
	if (_do_rotate == 1) then
		local mx
		local my
		mx, my = gh_input.mouse_getpos()
		
		if ((mx ~= _camera_old_mouse_x) or (my ~= _camera_old_mouse_y)) then
			_rotate_camera_orbit_v2(camera, mx, my, cam_lookat)
		end
	end
end


function GH_InitCameraRotation(camera, cam_lookat, pitch, yaw)
	_init_camera_angles(camera, pitch, yaw)
	_rotate_camera_position_around_point_v2(camera, cam_lookat, pitch, yaw, 0)
end
