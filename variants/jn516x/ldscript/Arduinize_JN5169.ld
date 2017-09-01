/* Set minimum heap size unless it is already set from App_Stack_Size.ld */
INCLUDE App_Stack_Size.ld
INCLUDE AppBuildStart.ld
INCLUDE AppBuildExceptionHandlers.ld
_minimum_heap_size = __minimum_heap_size;
INCLUDE Arduinize_JN516x.ld
