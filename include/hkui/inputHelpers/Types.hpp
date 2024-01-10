#pragma once

namespace inputHelpers
{
enum class Event
{
    MouseButton,
    MouseButton2,
    MouseMove,
    WindowResize,
    RenderDone,  //TODO: Artifact of design. Shall be removed later
    ItemsDrop
};
}