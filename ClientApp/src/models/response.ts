import type { Todo } from "./todo";

export interface Rseponse {
    status: number;
    data: Todo | undefined;
}