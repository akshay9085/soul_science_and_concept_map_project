<?php
// function textbook_companion_book_run_form($form, &$form_state)
function textbook_companion_book_run_form($form, &$form_state)
{
	$url_category_id = (int) arg(3);
	$url_subcategory_id = (int) arg(2);
	$url_proposal_id_id = (int) arg(1);
	if ($url_proposal_id_id != NULL && $url_subcategory_id != NULL && $url_category_id != NULL)
	{
		$category_default_value = $url_category_id;
		$subcategory_default_value = $url_subcategory_id;
	} //$url_proposal_id_id
	else
	{
		$category_default_value = 0;
		$subcategory_default_value = 0;
	}
	if ($url_proposal_id_id && $url_subcategory_id && $url_category_id)
	{
		$form['category'] = array(
			'#type' => 'select',
			'#title' => t('Category'),
			'#options' => _list_of_display_category(),
			'#default_value' => $category_default_value,
			// '#ajax' => array(
			// 	'callback' => 'ajax_subcategory_list_callback'
			// ),
			'#validated' => TRUE
		);
		$form['subcategory'] = array(
			'#type' => 'select',
			'#title' => t('Sub Category'),
			'#options' => _list_of_display_subcategory(),
			'#default_value' => $subcategory_default_value,
			// '#ajax' => array(
			// 	'callback' => 'ajax_book_list_callback'
			// ),
			// '#prefix' => '<div id="ajax-subcategory-list-replace">',
			// '#suffix' => '</div>',
			'#validated' => TRUE,
			'#states' => array(
				'invisible' => array(
					':input[name="category"]' => array(
						'value' => 0
					)
				)
			)
		);
		$book_default_value = $url_proposal_id_id;
		$form['science_and_concept_map'] = array(
			'#type' => 'select',
			'#title' => t('Title of the science and concept map'),
			'#options' => _list_of_science_and_concept_map(),
			'#default_value' => $book_default_value,
			// '#default_value' => $selected,
			// '#ajax' => array(
			// 	'callback' => 'ajax_chapter_list_callback'
			// )
		);
		
		$form['book_details'] = array(
			'#type' => 'item',
			'#markup' => '<div id="ajax-book-details-replace">' . '</div>'
		);
		
		// $form['selected_science_and_concept_map'] = array(
		// 	'#type' => 'item',
		// 	'#markup' => '<div id="ajax_selected_science_and_concept_map">' . l('Download Abstract', "science-and-concept-map-project/download/abstract-file/" . $book_default_value) . '<br>' . l('Download science and concept map', 'science-and-concept-map-project/full-download/project/' . $book_default_value) . '</div>'
		// );	
	} //$url_proposal_id_id
	else
	{
		$form['category'] = array(
			'#type' => 'select',
			'#title' => t('Category'),
			'#options' => _list_of_display_category(),
			'#default_value' => $category_default_value,
			// '#ajax' => array(
			// 	'callback' => 'ajax_subcategory_list_callback'
			// ),
			'#validated' => TRUE
		);
		//var_dump(_list_of_subcategory($category_default_value));
		$form['subcategory'] = array(
			'#type' => 'select',
			'#title' => t('Sub Category'),
			'#options' => _list_of_display_subcategory(),
			'#default_value' => $subcategory_default_value,
			// '#ajax' => array(
			// 	'callback' => 'ajax_book_list_callback'
			// ),
			// '#prefix' => '<div id="ajax-subcategory-list-replace">',
			// '#suffix' => '</div>',
			'#validated' => TRUE,
		);
		$form['science_and_concept_map'] = array(
			'#type' => 'select',
			'#title' => t('Title of the science and concept map'),
			'#options' => _list_of_science_and_concept_map($category_default_value, $subcategory_default_value),
			// '#default_value' => $selected,
			// '#ajax' => array(
			// 	'callback' => 'ajax_chapter_list_callback'
			// ),
			'#validated' => TRUE,
			// '#prefix' => '<div id="ajax-book-list-replace">',
			// '#suffix' => '</div>'
			
		);
		$book_default_value = $url_proposal_id_id;
		//book details and contributor details
		$form['book_details'] = array(
			'#type' => 'item',
			'#markup' => '<div id="ajax-book-details-replace">' . _html_book_info($book_default_value) . '</div>'
		);
		
		// $form['book_details'] = array(
		// 	'#type' => 'item',
		// 	'#markup' => '<div id="ajax-book-details-replace"></div>'
		// );	
		// $form['selected_science_and_concept_map'] = array(
		// 	'#type' => 'item',
		// 	'#markup' => '<div id="ajax_selected_science_and_concept_map">' . l('Download Abstract', "science-and-concept-map-project/download/abstract-file/" . $book_default_value) . '<br>' . l('Download science and concept map', 'science-and-concept-map-project/full-download/project/' . $book_default_value) . '</div>'
		// );	
	}
	return $form;
}



/****************for css************** */

function bootstrap_table_format($headers, $rows)
{
	$thead = "";
	$tbody = "";
	foreach ($headers as $header)
	{
		$thead .= "<th>{$header}</th>";
	} //$headers as $header
	foreach ($rows as $row)
	{
		$tbody .= "<tr>";
		foreach ($row as $data)
		{
			$tbody .= "<td>{$data}</td>";
		} //$row as $data
		$tbody .= "</tr>";
	} //$rows as $row
	$table = "
            <table class='table table-bordered table-hover' style='margin-left:-140px'>
                <thead>{$thead}</thead>
                <tbody>{$tbody}</tbody>
            </table>
        ";
	return $table;
}

function _list_of_display_category()
{
    $category[0] = "Please select";
        $query = db_select('class_table');
        $query->fields('class_table');
        $query->orderBy('id', 'ASC');
        $category_list = $query->execute();
    while ($category_list_data = $category_list->fetchObject())
    {
        $category[$category_list_data->class_name] = $category_list_data->class_name;
    } //$category_list_data = $category_list->fetchObject()
    return $category;
}

function _list_of_display_subcategory()
{
    $subcategory[0] = "Please select";
    $subcategory[1] = "Examples from NCERT Books";
    $subcategory[2] = "Examples from other sources";

      /*  $query = db_select('soul_science_and_concept_map_proposal','po');
        $query->distinct();
        $query->fields('po');
        $query->isNotNull('po.is_ncert_book');
        // $query->orderBy('ASC');
        $subcategory_list = $query->execute();
    while ($subcategory_list_data = $subcategory_list->fetchObject())
    {
        $subcategory[$subcategory_list_data->is_ncert_book] = $subcategory_list_data->is_ncert_book;
    } //$category_list_data = $category_list->fetchObject()*/
    return $subcategory;
}


//$category_default_value, $subcategory_default_value
function _list_of_science_and_concept_map($preference_id)
{
	$science_and_concept_map_titles = array(
		'0' => 'Please select...'
	);
	// if($category_default_value != 0 && $subcategory_default_value !=0){

	// 	//$lab_titles_q = db_query("SELECT * FROM {science_and_concept_map_proposal} WHERE solution_display = 1 ORDER BY lab_title ASC");
	// 	$query = db_select('soul_science_and_concept_map_proposal');
	// 	$query->fields('soul_science_and_concept_map_proposal');
	// 	$query->condition('approval_status', 3);
	// 	$query->condition('year_of_study', $category_default_value);
	// 	$query->condition('is_ncert_book', $subcategory_default_value);
		
	// 	$query->orderBy('project_title', 'ASC');
	// 	$science_and_concept_map_titles_q = $query->execute();
	// }
	// else{
		$query = db_select('soul_science_and_concept_map_proposal');
		$query->fields('soul_science_and_concept_map_proposal');
		$query->condition('approval_status', 3);
		
		$query->orderBy('project_title', 'ASC');
		$science_and_concept_map_titles_q = $query->execute();
	// }
	if ($science_and_concept_map_titles_q->rowCount() != 0)
	{
	while ($science_and_concept_map_titles_data = $science_and_concept_map_titles_q->fetchObject()) {
		$science_and_concept_map_titles[$science_and_concept_map_titles_data->id] = $science_and_concept_map_titles_data->project_title . ' (Proposed by ' . $science_and_concept_map_titles_data->name_title . ' ' . $science_and_concept_map_titles_data->contributor_name . ')';
	} //$science_and_concept_map_titles_data = $science_and_concept_map_titles_q->fetchObject()
}
	else
	{
		$science_and_concept_map_titles[0]= "There are no books availabe in this category";
	}
	return $science_and_concept_map_titles;
}

function _book_information($preference_id)
{
	$query = db_select('soul_science_and_concept_map_proposal','proposal');
	$query->fields('preference', array(
		'book',
		'author',
		'isbn',
		'publisher',
		'edition',
		'book_year'
	));
	$query->fields('proposal', array(
		'year_of_study'
	));
	$query->leftJoin('soul_science_and_concept_map_textbook_details', 'preference', 'proposal.id = preference.proposal_id');
	$query->condition('preference.id', $preference_id);
	
	$book_data = $query->execute()->fetchObject();
	return $book_data;
}


function _html_book_info($preference_id)
{
	/*$book_details = db_fetch_object(db_query("SELECT 
    preference.book AS preference_book,
    preference.author AS preference_author,
    preference.isbn AS preference_isbn,
    preference.publisher AS preference_publisher,
    preference.edition AS preference_edition,
    preference.book_year AS preference_year,
    proposal.year_of_study AS proposal_year_of_study
FROM 
    soul_science_and_concept_map_proposal AS proposal
LEFT JOIN 
    soul_science_and_concept_map_textbook_details AS preference
ON 
    proposal.id = preference.proposal_id
WHERE 
    preference.id = :preference_id
));*/
	$query = db_select('soul_science_and_concept_map_proposal', 'proposal');
	$query->addField('preference', 'book', 'preference_book');
	$query->addField('preference', 'author', 'preference_author');
	$query->addField('preference', 'isbn', 'preference_isbn');
	$query->addField('preference', 'publisher', 'preference_publisher');
	$query->addField('preference', 'edition', 'preference_edition');
	$query->addField('preference', 'book_year', 'preference_year');
	$query->addField('proposal', 'year_of_study', 'proposal_year_of_study');
	$query->addField('proposal', 'name_title', 'proposal_name_title');
	$query->addField('proposal', 'contributor_name', 'proposal_contributorname');
	// $query->addField('proposal', 'reviewer', 'proposal_reviewer');
	$query->addField('proposal', 'project_title', 'proposal_project_title');
	$query->addField('proposal', 'category', 'proposal_category');
	$query->addField('proposal', 'university', 'proposal_university');
	$query->fields('proposal', array(
		'year_of_study',
		'name_title',
		'contributor_name',
		'project_title',
		'category',
		'university'
		
	));
	$query->leftJoin('soul_science_and_concept_map_textbook_details', 'preference', 'proposal.id = preference.proposal_id');
	$query->fields('preference', array(
		'book',
		'author',
		'isbn',
		'publisher',
		'edition',
		'book_year'
	));
	$query->condition('preference.id', $preference_id);
	$book_details = $query->execute()->fetchObject();
	$html_data = '';
	if ($book_details)
	{
		if ($book_details->preference_author == "None")
		{
			$html_data = '<table cellspacing="1" cellpadding="1" border="0" style="width: 100%;" valign="top">' . 
			'<tr>' . 
				'<td style="width: 35%;">' . 
					'<span style="color: rgb(128, 0, 0);"><strong>About the Book</strong></span>' . 
				'</td>' . 
				'<td style="width: 35%;">' . 
					'<span style="color: rgb(128, 0, 0);"><strong>About the Contributor</strong></span>' . 
				'</td>' . 
			'</tr>' . 
			'<tr>' . 
				'<td valign="top">' . 
					'<ul>' . 
						'<li><strong>Title of the Book:</strong> ' . $book_details->preference_book . '</li>' . 
						'<li><strong>Publisher:</strong> ' . $book_details->preference_publisher . '</li>' . 
						'<li><strong>Year:</strong> ' . $book_details->preference_year . '</li>' . 
						'<li><strong>Edition:</strong> ' . $book_details->preference_edition . '</li>' . 
						'<li><strong>ISBN:</strong> ' . $book_details->preference_isbn . '</li>' . 
						'<li><strong>Year of Study: </strong>' . $book_details->proposal_year_of_study  . '</li>' .
					'</ul>' . 
				'</td>' . 
				'<td valign="top">' . 
					'<ul>' .
					'<li><strong>Proposer Name:</strong> ' . $book_details->name_title . ' ' . $book_details->contributor_name . '</li>' . '<li><strong>Title of the science and concept map:</strong> ' . '<span style="color: #5D125D;">' .$book_details->project_title. '</li>' . '<li><strong>University:</strong> ' . $book_details->university . '</li>' . '<li><strong>Category:</strong> ' . $book_details->category . '</li>' . 
					'</ul>' . 
				'</td>' . 
			'</tr>' . 
		'</table>'
		;
		} //$book_details->proposal_faculty == "None"
		else
		{
			$html_data = '<table cellspacing="1" cellpadding="1" border="0" style="width: 100%;" valign="top">' . 
			'<tr>' . 
				'<td style="width: 35%;">' . 
					'<span style="color: rgb(128, 0, 0);"><strong>About the Book</strong></span>' . 
				'</td>' . 
				'<td style="width: 35%;">' . 
					'<span style="color: rgb(128, 0, 0);"><strong>About the Contributor</strong></span>' . 
				'</td>' . 
			'</tr>' . 
			'<tr>' . 
				'<td valign="top">' . 
					'<ul>' . 
						'<li><strong>Title of the Book:</strong> ' . $book_details->preference_book . '</li>' . 
						'<li><strong>Author:</strong> ' . $book_details->preference_author . '</li>' .
						'<li><strong>Publisher:</strong> ' . $book_details->preference_publisher . '</li>' . 
						'<li><strong>Year:</strong> ' . $book_details->preference_year . '</li>' . 
						'<li><strong>Edition:</strong> ' . $book_details->preference_edition . '</li>' . 
						'<li><strong>ISBN:</strong> ' . $book_details->preference_isbn . '</li>' . 
						'<li><strong>Year of Study: </strong>' . $book_details->proposal_year_of_study  . '</li>' . 
					'</ul>' . 
				'</td>' . 
				'<td valign="top">' . 
					'<ul>' .
					'<li><strong>Proposer Name:</strong> ' . $preference_id->name_title . ' ' . $preference_id->contributor_name . '</li>' .
					'<li><strong>Title of the science and concept map:</strong> <span style="color: #5D125D;">' . $preference_id->project_title . '</span></li>' .
					'<li><strong>University:</strong> ' . $book_details->university . '</li>' .
					'<li><strong>Category:</strong> ' . $book_details->category . '</li>' .
					 
					'</ul>' . 
				'</td>' . 
			'</tr>' . 
		'</table>'
		;
		}
	} //$book_details
	return $html_data;
}
