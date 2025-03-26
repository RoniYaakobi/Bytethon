# Markdown Styling Guide

## **This page is a general styling guide for markdown, mainly for myself, feel free to read or copy it.**

If you are using VSCode like me, in order to preview how the markdown page will look, you can use the extension [`Markdown Preview Enhanced`](https://marketplace.visualstudio.com/items?itemName=shd101wyy.markdown-preview-enhanced) , then when in the file you want to preview, press `Ctrl + Shift + V`. Alternatively you can use VSCode's built-in preview by right-clicking on the `.md` file and selecting **"Open Preview"**, but keep in mind that from my experience, sometimes it doesn't know how to render elements like footnotes and task lists properly.

## 1. Headers
Headers are used to structure your document and can be created using `#` symbols.

### Header Levels:
# Header 1
## Header 2
### Header 3
#### Header 4
##### Header 5
###### Header 6

## 2. Emphasis

### Italic:
Use single asterisks (`*`) or underscores (`_`) for italic text.
*Italic*  
_Italic_

### Bold:
Use double asterisks (`**`) or double underscores (`__`) for bold text.
**Bold**  
__Bold__

### Bold and Italic:
Use triple asterisks (`***`) for bold and italic combined.
***Bold and Italic***

## 3. Lists

### Unordered Lists:
Use asterisks (`*`), plus signs (`+`), or hyphens (`-`) followed by a space.

- Item 1
- Item 2
    - Subitem 1
    - Subitem 2
- Item 3

### Ordered Lists:
Simply use numbers followed by a period.

1. First item
2. Second item
3. Third item

## 4. Links

You can create hyperlinks by wrapping the link text in square brackets (`[]`) and the URL in parentheses (`()`).

[Google](https://www.google.com)

## 5. Images
Images are similar to links but with an exclamation mark (`!`) before the square brackets.

![Alt text](https://example.com/image.png)

## 6. Code

### Inline Code:
Use backticks (`` ` ``) for inline code.

`printf("Hello, world!");`

### Code Blocks:
Use triple backticks (```` ``` ````) for code blocks.

```
#include <stdio.h>

int main() {
    printf("Hello, world!\n");
    return 0;
}
```

You can also specify the language after the third backtick like so (`` ```python ``)

```python
print("Hello, world!")
foo, bar = 10,15
```

## 7. Blockquotes
Blockquotes are created using the greater-than symbol (>).
> This is a blockquote.
> It is used for quotes, citations, or to highlight text.

## 8. Horizontal Rules
Horizontal rules are created with three hyphens (---), asterisks (***), or underscores (___).
---

## 9. Tables
Tables are created using pipes (|) to separate columns and hyphens (-) to separate headers from rows.

| Header 1 |  Header 2 | Header 3|
|----------|-----------|---------|
| Row 1 | Data 1 |	More Data |
| Row 2 | Data 2 |	More Data |
| Row 3 | Data 3 |	More Data |

## 10. Task Lists
Task lists are created using square brackets ([ ]) for tasks and [x] for completed tasks.

- [ ] Task 1
- [x] Task 2 (Completed)
- [ ] Task 3

## 11. Strikethrough
Strikethrough text is created with double tildes (~~).

~~This text is strikethrough~~

## 12. Footnotes
Footnotes are created using square brackets with a caret ([^1]) and then defined elsewhere in the document.

This is a footnote example[^1].

[^1]: This is footnote text.
## 13. HTML Elements
You can also use raw HTML elements for more advanced formatting.

<b>This is bold using HTML</b>