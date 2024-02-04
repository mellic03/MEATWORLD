function ScriptEntry()

    if EventSystem.keyTapped(EventSystem.KEY_ESCAPE) then
        local captured = EventSystem.mouseCaptured();

        if captured then
            EventSystem.mouseCapture(false)
        else
            EventSystem.mouseCapture(true)
        end
    end

end

