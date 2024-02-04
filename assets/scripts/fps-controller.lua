function ScriptEntry( subject )

    if subject == nil then
        return;
    end

    if EventSystem.mouseCaptured() == false then
        return;
    end


    local dtime = Engine.deltaTime();
    local speed = 2.25 * dtime;

    if EventSystem.keyDown(EventSystem.KEY_LSHIFT) then
        speed = speed * 4.0;
    end

    if EventSystem.keyDown(EventSystem.KEY_W) then
        TransformSys.translateLocal(subject, 0.0, 0.0, speed);
    end

    if EventSystem.keyDown(EventSystem.KEY_S) then
        TransformSys.translateLocal(subject, 0.0, 0.0, -speed);
    end

    if EventSystem.keyDown(EventSystem.KEY_A) then
        TransformSys.translateLocal(subject, -speed, 0.0, 0.0);
    end

    if EventSystem.keyDown(EventSystem.KEY_D) then
        TransformSys.translateLocal(subject, speed, 0.0, 0.0);
    end

    if EventSystem.keyDown(EventSystem.KEY_SPACE) then
        TransformSys.translate(subject, 0, speed, 0);
    end

    if EventSystem.keyDown(EventSystem.KEY_LCTRL) then
        TransformSys.translate(subject, 0, -speed, 0);
    end


    local dx = EventSystem.deltaMouseX();
    local dy = EventSystem.deltaMouseY();

    TransformSys.yawGlobal(subject, -0.001 * dx);
    TransformSys.pitch(subject, -0.001 * dy);


end

