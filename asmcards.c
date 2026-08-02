#include <furi.h>
#include <gui/gui.h>
#include "cards.h"
#include <stdlib.h>
#include <gui/elements.h>

#define PHASE_ANSWER 0
#define PHASE_CORRECT 1
#define PHASE_WRONG 2

#define LIST_ROWS 6

typedef struct {
    Screen screen;
    Mode mode;
    int mode_cursor;
    int cat_cursor;
    int selected;
    int active_category;
    int active_deck;
    int order[64];
    int position;
    // quiz
    int quiz_cursor;   // 0 or 1, which option is highlighted
    int opt[2];        // card indices of the two options
    int answer_pos;    // which option (0/1) is correct
    int quiz_phase;    // answering / correct / wrong
} AppState;

static const Deck* current_deck(const AppState* state) {
    return &categories[state->active_category].decks[state->active_deck];
}

// first visible row so the cursor always stays on screen
static int list_first(int cursor) {
    if(cursor < LIST_ROWS) return 0;
    return cursor - LIST_ROWS + 1;
}

static void shuffle_deck(AppState* state, int count) {
    for(int i = 0; i < count; i++) {
        state->order[i] = i;
    }
    for(int i = count - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = state->order[i];
        state->order[i] = state->order[j];
        state->order[j] = tmp;
    }
    state->position = 0;
}

// build one quiz question from the current card (order[position])
static void setup_quiz(AppState* state) {
    int count = current_deck(state)->count;
    int answer = state->order[state->position];
    int distractor;
    do {
        distractor = rand() % count;
    } while(distractor == answer);

    state->answer_pos = rand() % 2;
    state->opt[state->answer_pos] = answer;
    state->opt[1 - state->answer_pos] = distractor;
    state->quiz_cursor = 0;
    state->quiz_phase = PHASE_ANSWER;
}

// draw screen, called every time when refresh
static void draw_callback(Canvas* canvas, void* context) {
    AppState* state = context;
    canvas_clear(canvas);

    switch(state->screen) {
    case ScreenTitle:
        canvas_set_font(canvas, FontPrimary);
        canvas_draw_str(canvas, 10, 26, "ASM Cards");
        canvas_set_font(canvas, FontSecondary);
        canvas_draw_str(canvas, 10, 40, "x86-64 instructions");
        canvas_draw_str(canvas, 10, 54, "Press OK to start");
        break;
    case ScreenCategory: {
        canvas_set_font(canvas, FontSecondary);
        int first = list_first(state->cat_cursor);
        for(int row = 0; row < LIST_ROWS && first + row < (int)COUNT(categories); row++) {
            int i = first + row;
            if(i == state->cat_cursor) {
                canvas_draw_str(canvas, 2, 12 + row * 9, ">");
            }
            canvas_draw_str(canvas, 12, 12 + row * 9, categories[i].label);
        }
        break;
    }
    case ScreenMenu: {
        const Category* cat = &categories[state->active_category];
        canvas_set_font(canvas, FontSecondary);
        int first = list_first(state->selected);
        for(int row = 0; row < LIST_ROWS && first + row < cat->count; row++) {
            int i = first + row;
            if(i == state->selected) {
                canvas_draw_str(canvas, 2, 12 + row * 9, ">");
            }
            canvas_draw_str(canvas, 12, 12 + row * 9, cat->decks[i].label);
        }
        break;
    }
    case ScreenMode: {
        canvas_set_font(canvas, FontSecondary);
        int first = list_first(state->mode_cursor);
        for(int row = 0; row < LIST_ROWS && first + row < (int)COUNT(modes); row++) {
            int i = first + row;
            if(i == state->mode_cursor) {
                canvas_draw_str(canvas, 2, 12 + row * 9, ">");
            }
            canvas_draw_str(canvas, 12, 12 + row * 9, modes[i].label);
        }
        break;
    }
    case ScreenCard: {
        int card_index = state->order[state->position];
        const Card* card = &current_deck(state)->cards[card_index];
        canvas_set_font(canvas, FontPrimary);
        canvas_draw_str(canvas, 10, 30, card->name);
        canvas_set_font(canvas, FontSecondary);
        canvas_draw_str(canvas, 10, 45, card->meaning);
        break;
    }
    case ScreenQuiz: {
        int q_index = state->order[state->position];
        const Card* q = &current_deck(state)->cards[q_index];
        int name_to_meaning = (state->mode == ModeQuizNameToMeaning);

        if(state->quiz_phase == PHASE_CORRECT) {
            canvas_set_font(canvas, FontPrimary);
            canvas_draw_str(canvas, 10, 35, "Correct!");
        } else if(state->quiz_phase == PHASE_WRONG) {
            canvas_set_font(canvas, FontPrimary);
            canvas_draw_str(canvas, 2, 16, "Wrong");
            canvas_set_font(canvas, FontSecondary);
            canvas_draw_str(canvas, 2, 32, q->name);
            canvas_draw_str(canvas, 2, 44, q->meaning);
            canvas_draw_str(canvas, 2, 62, "OK: next");
        } else {
            canvas_set_font(canvas, FontSecondary);
            const char* prompt = name_to_meaning ? q->name : q->meaning;
            canvas_draw_str(canvas, 2, 12, prompt);
            for(int i = 0; i < 2; i++) {
                const Card* o = &current_deck(state)->cards[state->opt[i]];
                const char* text = name_to_meaning ? o->meaning : o->name;
                if(i == state->quiz_cursor) {
                    canvas_draw_str(canvas, 2, 34 + i * 12, ">");
                }
                canvas_draw_str(canvas, 10, 34 + i * 12, text);
            }
        }
        break;
    }
    case ScreenDetail: {
        int card_index = state->order[state->position];
        const Card* card = &current_deck(state)->cards[card_index];
        canvas_set_font(canvas, FontPrimary);
        canvas_draw_str(canvas, 2, 12, card->name);
        canvas_set_font(canvas, FontSecondary);
        if(card->detail) {
            elements_multiline_text(canvas, 2, 26, card->detail);
        } else {
            canvas_draw_str(canvas, 2, 26, "No detail yet");
        }
        break;
    }
    }
}

// get input
static void input_callback(InputEvent* event, void* context) {
    FuriMessageQueue* queue = context;
    furi_message_queue_put(queue, event, FuriWaitForever);
}

int32_t asmcards_app(void* p) {
    UNUSED(p);

    AppState state;
    state.screen = ScreenTitle;
    state.cat_cursor = 0;
    state.active_category = 0;
    state.selected = 0;
    state.mode_cursor = 0;

    srand(DWT->CYCCNT);

    FuriMessageQueue* queue = furi_message_queue_alloc(8, sizeof(InputEvent));

    ViewPort* view_port = view_port_alloc();
    view_port_draw_callback_set(view_port, draw_callback, &state);
    view_port_input_callback_set(view_port, input_callback, queue);

    Gui* gui = furi_record_open(RECORD_GUI);
    gui_add_view_port(gui, view_port, GuiLayerFullscreen);

    InputEvent event;
    bool running = true;
    while(running) {
        if(furi_message_queue_get(queue, &event, FuriWaitForever) == FuriStatusOk) {
            if(event.type == InputTypePress) {
                switch(event.key) {
                case InputKeyBack:
                    if(state.screen == ScreenTitle || state.screen == ScreenCategory) {
                        running = false;
                    } else if(state.screen == ScreenMenu) {
                        state.screen = ScreenCategory;
                    } else if(state.screen == ScreenMode) {
                        state.screen = ScreenMenu;
                    } else if(state.screen == ScreenCard) {
                        state.screen = ScreenMenu;
                    } else if(state.screen == ScreenQuiz) {
                        state.screen = ScreenMode;
                    } else if(state.screen == ScreenDetail) {
                        state.screen = ScreenCard;
                    }
                    break;
                case InputKeyOk:
                    if(state.screen == ScreenTitle) {
                        state.screen = ScreenCategory;
                    } else if(state.screen == ScreenCategory) {
                        state.active_category = state.cat_cursor;
                        state.selected = 0;
                        state.screen = ScreenMenu;
                    } else if(state.screen == ScreenMenu) {
                        state.active_deck = state.selected;
                        shuffle_deck(&state, current_deck(&state)->count);
                        state.screen = ScreenMode;
                        state.mode_cursor = 0;
                    } else if(state.screen == ScreenMode) {
                        state.position = 0;
                        if(state.mode_cursor == 0) {
                            state.mode = ModeCards;
                            state.screen = ScreenCard;
                        } else {
                            state.mode = (state.mode_cursor == 1)
                                             ? ModeQuizNameToMeaning
                                             : ModeQuizMeaningToName;
                            setup_quiz(&state);
                            state.screen = ScreenQuiz;
                        }
                    } else if(state.screen == ScreenQuiz) {
                        if(state.quiz_phase == PHASE_ANSWER) {
                            if(state.quiz_cursor == state.answer_pos) {
                                state.quiz_phase = PHASE_CORRECT;
                            } else {
                                state.quiz_phase = PHASE_WRONG;
                            }
                        } else {
                            state.position = state.position + 1;
                            if(state.position >= current_deck(&state)->count) {
                                shuffle_deck(&state, current_deck(&state)->count);
                            }
                            setup_quiz(&state);
                        }
                    } else if(state.screen == ScreenCard) {
                        state.screen = ScreenDetail;
                    } else if(state.screen == ScreenDetail) {
                        state.screen = ScreenCard;
                    }
                    break;
                case InputKeyUp:
                    if(state.screen == ScreenCategory) {
                        if(state.cat_cursor != 0) {
                            state.cat_cursor = state.cat_cursor - 1;
                        } else {
                            state.cat_cursor = COUNT(categories) - 1;
                        }
                    } else if(state.screen == ScreenMenu) {
                        if(state.selected != 0) {
                            state.selected = state.selected - 1;
                        } else {
                            state.selected = categories[state.active_category].count - 1;
                        }
                    } else if(state.screen == ScreenMode) {
                        if(state.mode_cursor != 0) {
                            state.mode_cursor = state.mode_cursor - 1;
                        } else {
                            state.mode_cursor = COUNT(modes) - 1;
                        }
                    } else if(state.screen == ScreenQuiz) {
                        if(state.quiz_phase == PHASE_ANSWER) {
                            state.quiz_cursor = (state.quiz_cursor == 0) ? 1 : 0;
                        }
                    }
                    break;
                case InputKeyDown:
                    if(state.screen == ScreenCategory) {
                        if(state.cat_cursor != (int)COUNT(categories) - 1) {
                            state.cat_cursor = state.cat_cursor + 1;
                        } else {
                            state.cat_cursor = 0;
                        }
                    } else if(state.screen == ScreenMenu) {
                        if(state.selected != categories[state.active_category].count - 1) {
                            state.selected = state.selected + 1;
                        } else {
                            state.selected = 0;
                        }
                    } else if(state.screen == ScreenMode) {
                        if(state.mode_cursor != COUNT(modes) - 1) {
                            state.mode_cursor = state.mode_cursor + 1;
                        } else {
                            state.mode_cursor = 0;
                        }
                    } else if(state.screen == ScreenQuiz) {
                        if(state.quiz_phase == PHASE_ANSWER) {
                            state.quiz_cursor = (state.quiz_cursor == 0) ? 1 : 0;
                        }
                    }
                    break;
                case InputKeyRight:
                    if(state.screen == ScreenCard) {
                        state.position = state.position + 1;
                        if(state.position >= current_deck(&state)->count) {
                            shuffle_deck(&state, current_deck(&state)->count);
                        }
                    }
                    break;
                case InputKeyLeft:
                    if(state.screen == ScreenCard) {
                        if(state.position > 0) {
                            state.position = state.position - 1;
                        }
                    }
                    break;
                default:
                    break;
                }
                view_port_update(view_port);
            }
        }
    }

    gui_remove_view_port(gui, view_port);
    furi_record_close(RECORD_GUI);
    view_port_free(view_port);
    furi_message_queue_free(queue);

    return 0;
}