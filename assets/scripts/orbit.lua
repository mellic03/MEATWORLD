local vx = 5.1;
local vy = 0.0;
local vz = 0.0;


function ScriptEntry( subject )

    if subject == nil then
        return;
    end

    local dtime = Engine.deltaTime();
    local speed = dtime;

    local x = TransformSys.getX(subject);
    local y = TransformSys.getY(subject);
    local z = TransformSys.getZ(subject);

    local dx = 0.0 - x;
    local dy = 0.0 - y;
    local dz = 0.0 - z;

    local length = math.sqrt(dx*dx + dy*dy + dz*dz);

    vx = vx + speed * length * dx;
    vy = vy + speed * length * dy;
    vz = vz + speed * length * dz;

    TransformSys.translate(subject, vx, vy, vz);

end

