<script setup lang="ts">
import { ref } from 'vue'
import { Todo } from '../models/todo'

const props = defineProps<{ todo: Todo, fetchTodos: () => Promise }>()

async function updateTodo(done: boolean) {
  const response = await fetch(`/api/todo/${props.todo.id}`, {
    method: "PUT",
    headers: {
      "Content-Type": "application/json",
    },
    body: JSON.stringify({
      "done": done
    }),
  });
}

async function deleteTodo() {  
  const response = await fetch(`/api/todo/${props.todo.id}`, {
    method: "DELETE"
  });

  await props.fetchTodos();
}

async function onDoneChanged(event) {
  event.preventDefault()
  await updateTodo(event.target.checked)
}
</script>

<template>
  <div>
    <h3>{{ todo.task }}</h3>
  </div>
  <div>
  <input type="checkbox" v-model="todo.done" @change="onDoneChanged" />
  
  <label>Done</label>
  </div>
  <div>
  <button @click="deleteTodo()">Delete</button>
  </div>
</template>

<style scoped>
</style>
