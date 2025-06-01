<script setup lang="ts">
import { ref } from 'vue'
import { Todo } from '../models/todo'

const props = defineProps<{ fetchTodos: () => Promise }>()

const message = ref("")

async function postTodo() {
  console.log("will post message", message.value);
  const response = await fetch(`/api/todo`, {
    method: "POST",
    headers: {
      "Content-Type": "application/json",
    },
    body: JSON.stringify({
      "task": message.value,
      "done": false
    }),
  });
  
  message.value = "";
  await props.fetchTodos();
}

</script>

<template>
  <div>
    <textarea v-model="message" />
  </div>
  <div>
    <button @click="postTodo()">Create</button>
  </div>
</template>

<style scoped>
</style>
