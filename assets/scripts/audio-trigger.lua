function ScriptEntry( subject, audio_obj )

    if subject == nil or audio_obj == nil then
        return;
    end

    if EventSystem.mouseCaptured() == false then
        return;
    end


    local sound = false;

    if EventSystem.keyDown(EventSystem.KEY_W) then
        sound = true;
    end

    if EventSystem.keyDown(EventSystem.KEY_S) then
        sound = true;
    end

    if EventSystem.keyDown(EventSystem.KEY_A) then
        sound = true;
    end

    if EventSystem.keyDown(EventSystem.KEY_D) then
        sound = true;
    end


    if sound == true then
        AudioSys.playSound(audio_obj, true);
    else
        AudioSys.stopSound(audio_obj);
    end


end

