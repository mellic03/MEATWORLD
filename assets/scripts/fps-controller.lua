
function ScriptEntry( subject )

    if subject == nil then
        return;
    end

    if EventSystem.mouseCaptured() == false then
        return;
    end

    local dtime = Engine.deltaTime();
    local speed = 0.08;

    local dx = 0.0;
    local dy = 0.0;
    local dz = 0.0;
    local roll = 0.0;

    if EventSystem.keyDown(EventSystem.KEY_LSHIFT) then
        speed = 2.0 * speed;
    end

    if EventSystem.keyDown(EventSystem.KEY_W) then
        dz = dz + speed;
    end

    if EventSystem.keyDown(EventSystem.KEY_S) then
        dz = dz - speed;
    end

    if EventSystem.keyDown(EventSystem.KEY_A) then
        dx = dx - speed;
    end

    if EventSystem.keyDown(EventSystem.KEY_D) then
        dx = dx + speed;
    end

    if EventSystem.keyDown(EventSystem.KEY_SPACE) then
        dy = dy + speed;
    end

    if EventSystem.keyDown(EventSystem.KEY_LCTRL) then
        dy = dy - speed;
    end

    local mouseX = -0.035 * EventSystem.deltaMouseX();
    local mouseY = -0.035 * EventSystem.deltaMouseY();


    if EventSystem.keyDown(EventSystem.KEY_Q) then
        roll = roll - 32.0*dtime;
    end

    if EventSystem.keyDown(EventSystem.KEY_E) then
        roll = roll + 32.0*dtime;
    end


    if PhysXSys.hasCCT(subject) then
        PhysXSys.moveCCT(subject, dx, dy, dz);
    else
        if PlanetSys.actorHasPlanet(subject) then
            TransformSys.moveSurfaceUp(subject, 0.1*dy);
            TransformSys.moveSurfaceRight(subject, 0.1*dx);
            TransformSys.moveSurfaceFront(subject, 0.1*dz);
        else
            TransformSys.moveUp(subject, 0.1*dy);
            TransformSys.moveRight(subject, 0.1*dx);
            TransformSys.moveFront(subject, 0.1*dz);
        end
    end




    TransformSys.yaw(subject, mouseX);
    TransformSys.pitch(subject, mouseY);
    TransformSys.roll(subject, roll);


end

