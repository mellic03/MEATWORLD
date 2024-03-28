
function ScriptEntry( subject, target )

    if subject == nil or target == nil then
        return 0;
    end


    local dtime = Engine.deltaTime();
    local speed = 0.50;

    local dx = 0.0;
    local dy = 0.0;
    local dz = 0.0;


    if EventSystem.keyDown(EventSystem.KEY_I) then
        dz = dz + speed;
    end

    if EventSystem.keyDown(EventSystem.KEY_K) then
        dz = dz - speed;
    end

    if EventSystem.keyDown(EventSystem.KEY_J) then
        dx = dx - speed;
    end

    if EventSystem.keyDown(EventSystem.KEY_L) then
        dx = dx + speed;
    end

    if EventSystem.keyDown(EventSystem.KEY_SPACE) then
        dy = dy + speed;
    end

    if EventSystem.keyDown(EventSystem.KEY_LCTRL) then
        dy = dy - speed;
    end


    TransformSys.moveUp(subject, 0.1*dy);
    TransformSys.moveRight(subject, 0.1*dx);
    TransformSys.moveFront(subject, 0.1*dz);


    return 0;

end

