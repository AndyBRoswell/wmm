const Dict = {
    "写作素材管理器": "Writing Materials Manager",
    "架构图": "architecture diagram",
}

console.log('Reading translation file ...')

const ElementsTranslate = document.querySelectorAll('.tr')
console.log('Translating ' + ElementsTranslate.length + ' elements ...')
ElementsTranslate.forEach(e => {
    const TranslatedText = Dict[e.innerHTML]
    if (TranslatedText) e.innerHTML = TranslatedText
})
console.log('Translation completed.')
