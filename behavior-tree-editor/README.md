# Behaviac Behavior Tree Visual Editor

A web-based visual editor for creating and managing Behaviac behavior trees with proper XML import/export.

## Features

✅ **Behaviac Node Types:**
- **Selector** - Runs children until one succeeds (OR logic)
- **Sequence** - Runs children until one fails (AND logic)
- **Parallel** - Runs all children simultaneously
- **Action** - Leaf node that executes a method
- **Condition** - Leaf node that checks a condition
- **Not (Inverter)** - Decorator that flips child result
- **Loop** - Decorator that repeats child N times (-1 = infinite)
- **AlwaysSuccess** - Decorator that forces success
- **AlwaysFailure** - Decorator that forces failure

✅ **Editing:**
- Drag nodes from palette onto canvas
- Click and drag to move nodes
- Connect nodes by dragging from bottom ⚪ to top ⚪
- Edit node properties (comment, method, loop count)
- Delete nodes with × button (hover to see)

✅ **Navigation:**
- Mouse wheel to zoom in/out
- Middle-click + drag (or drag empty space) to pan
- Click nodes to select them

✅ **File Operations:**
- **Export XML** - Generate Behaviac-compatible XML
- **Import XML** - Load existing Behaviac behavior trees
- **Save JSON** - Save editor layout (positions + properties)
- **Load JSON** - Restore editor layout
- **Clear All** - Start fresh

## Usage

1. Open `index.html` in any modern browser (Chrome, Firefox, Safari, Edge)
2. Set tree name, agent type, and version in the top panel
3. Drag node types from the palette onto the canvas
4. Connect nodes by dragging from the bottom connector to the top of another node
5. Fill in properties:
   - **Comment**: Describe what the node does
   - **Method** (Action/Condition): The C++ method name to call
   - **Loop Count** (Loop decorator): Number of times to repeat (-1 = infinite)
6. Export XML when ready to use in Unreal Engine

## Behaviac XML Format

The editor exports XML in the standard Behaviac format:

```xml
<?xml version="1.0" encoding="utf-8"?>
<behavior name="MinionAI" agenttype="BehaviacTestMinion" version="5">
    <node class="Selector" id="1" comment="Combat or Patrol">
        <node class="Sequence" id="2" comment="Combat behavior">
            <node class="Condition" id="3" comment="Has target?">
                <property Method="HasTarget" />
            </node>
            <node class="Action" id="4" comment="Move to target">
                <property Method="MoveToTarget" />
            </node>
        </node>
        <node class="Action" id="5" comment="Patrol">
            <property Method="PatrolToGoal" />
        </node>
    </node>
</behavior>
```

Place exported XML files in your UE project's `Content/BehaviacData/` or `Content/AI/Behaviac/` folder.

## Integration with Unreal Engine

1. Ensure BehaviacPlugin is enabled in your project
2. Export XML from this editor
3. Place XML file in `Content/BehaviacData/` or `Content/AI/Behaviac/`
4. Reference the behavior tree from your AIController or agent component
5. Implement the methods (Actions/Conditions) in your agent class

## JSON Layout Format

The editor can also save/load JSON files that preserve the visual layout:

```json
{
  "treeName": "MinionAI",
  "agentType": "BehaviacTestMinion",
  "version": "5",
  "nodes": [
    {
      "id": 1,
      "type": "Selector",
      "x": 100,
      "y": 50,
      "comment": "Main decision",
      "method": "",
      "loopCount": -1,
      "children": [2, 5]
    }
  ],
  "connections": [...]
}
```

This is useful for saving your work-in-progress layouts. Use **Export XML** for the final Behaviac-compatible output.

## Color Coding

Nodes are color-coded by type:
- 🟠 Selector (orange)
- 🟢 Sequence (green)
- 🟣 Parallel (purple)
- 🔵 Action (blue)
- 🔴 Condition (red)
- 🔷 Not/Inverter (cyan)
- 🟡 Loop (yellow)
- 🍀 AlwaysSuccess (light green)
- 🔴 AlwaysFailure (pink)

## No Dependencies

Single HTML file - no frameworks, no build process, no npm install. Just open and use!

## Example Workflow

1. **Plan**: Sketch your AI behavior logic
2. **Build**: Drag nodes and connect them in the editor
3. **Document**: Add comments to explain each node's purpose
4. **Export**: Generate XML for Unreal Engine
5. **Implement**: Write the C++ methods referenced in Actions/Conditions
6. **Test**: Run in-game and iterate

## Tips

- **Start with a root node**: Usually a Selector or Sequence
- **Add comments**: They show up in exported XML and help document your logic
- **Use Selectors for fallbacks**: "Try A, if that fails, try B"
- **Use Sequences for steps**: "Do A, then B, then C"
- **Decorators modify behavior**: Loop repeats, Not inverts, Always* forces results
- **Actions do things**: Move, Attack, PlayAnimation, etc.
- **Conditions check state**: IsInRange, HasTarget, IsHealthLow, etc.

## Limitations

- Decorator nodes support one child only (Behaviac limitation)
- Node IDs must be unique integers
- Method names are case-sensitive (match your C++ exactly)

## Compatibility

Tested with:
- Behaviac UE5 Plugin (https://github.com/mickiedd/BehaviaU)
- Behaviac version 5
- Unreal Engine 5.x

---

Built for Crunch-master project 🐶
