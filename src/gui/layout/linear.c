#include <stdlib.h>

#include "../eventhandler.h"
#include "../event.h"
#include "../gui.h"
#include "linear.h"


static void paint(GUI* this) {
    // Fallback if for *some* reason the aux struct is null
    LinearAlignment alignment = HORIZONTAL;
    if (this->aux != NULL)
        alignment = ((LinearLayoutData*) this->aux)->alignment;

    return;
}


static void* resize(GUI* this, ResizeData* data) {
    // TODO: adjust layout of elements here
    return NULL;
}


GUI* GUI_linear_layout(GUI* frame, LinearAlignment alignment) {
    frame->paint = paint;
    frame->aux = malloc(sizeof(LinearLayoutData));

    if (frame->aux != NULL) {
        ((LinearLayoutData*) frame->aux)->alignment = alignment;
    }

    return frame;
}
